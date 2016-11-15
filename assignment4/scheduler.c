#include "scheduler.h"
#include <stdlib.h>
#include <stdio.h>
#define STACK_SIZE  1024*1024

struct queue ready_list;
struct queue done_list;
//struct queue blocked_list;
struct thread * current_thread;

//Function prototype for threads 
void thread_switch(struct thread * old, struct thread * new);
void thread_start(struct thread * old, struct thread * new);


void thread_wrap() {
    current_thread->initial_function(current_thread->initial_argument);
    current_thread->state = DONE;
    //Signal thread conditional variable
    condition_broadcast(&(current_thread->t_cond));
    yield();
}

int threadCount = 0;

//One time setup and initialization
void scheduler_begin(){
    //Allocate current thread
    current_thread = (struct thread*) malloc(sizeof(struct thread));
    current_thread->state = RUNNING;
    current_thread->threadId = threadCount;
    mutex_init(&(current_thread->t_mutex));
    condition_init(&(current_thread->t_cond));
    threadCount++;
    ready_list.head = NULL;
    ready_list.tail = NULL;
    done_list.head = NULL;
    done_list.tail = NULL;
    //blocked_list.head = NULL;
    //blocked_list.tail = NULL;
}

struct thread* thread_fork(void(*target)(void*), void * arg){
    //Allocate new thread control block and stack
    struct thread * new_thread = (struct thread*) malloc(sizeof(struct thread));
    new_thread->base_stack = (unsigned char*) malloc(STACK_SIZE);
    new_thread->stack_pointer = new_thread->base_stack + STACK_SIZE;
    new_thread->initial_function = target;
    new_thread->initial_argument = arg;
    mutex_init(&(new_thread->t_mutex));
    condition_init(&(new_thread->t_cond));
    new_thread->threadId = threadCount;
    threadCount++;
    //Switching context
    current_thread->state = READY;
    thread_enqueue(&ready_list, current_thread);
    new_thread->state = RUNNING;
    struct thread * tmp_thread = current_thread;
    current_thread = new_thread;
    thread_start(tmp_thread, current_thread);
    return new_thread;
}

void yield() {
    //Thread is runnable
    switch(current_thread->state){
        case RUNNING:
            current_thread->state = READY;
            thread_enqueue(&ready_list, current_thread);
            break;
        case DONE:
            thread_enqueue(&done_list, current_thread);
            break;
        case READY:
            //current thread cannot be ready
            printf("Error, current thread has READY state\n");
            exit(EXIT_FAILURE);
        case BLOCKED:
            //Does not enqueue into ready list
            //thread_enqueue(&blocked_list, current_thread);
            break;
        default:
            printf("Error, thread state %d not recognized\n", current_thread->state);
            exit(EXIT_FAILURE);
    }

    struct thread * tmp_thread = current_thread;

    //Make sure there are other threads to run
    //If no other threads, no need to context switch
    if(!is_empty(&ready_list)){
        current_thread = thread_dequeue(&ready_list);
        current_thread->state = RUNNING;
        //printf("Switching thread %d to thread %d\n", tmp_thread->threadId, current_thread->threadId);
        thread_switch(tmp_thread, current_thread);
    }
}

void scheduler_end(){
    //While the queue is not empty, we call yield
    while(!is_empty(&ready_list)){
        yield();
    }

    //After all threads have yielded, we free all thread memory
    //Note that there will be a memory leak if scheduler_end is never called
    while(!is_empty(&done_list)){
        struct thread * tmp_thread = thread_dequeue(&done_list);
        //Free allocated stack
        free(tmp_thread->base_stack);
        tmp_thread->stack_pointer = NULL;
        tmp_thread->base_stack = NULL;
        free(tmp_thread);
        tmp_thread = NULL;
    }
    //Free current thread block (main thread)
    free(current_thread);
    current_thread = NULL;
}

void mutex_init(struct mutex * i_mutex){
    i_mutex->held = 1;
    i_mutex->waiting_threads.head = NULL;
    i_mutex->waiting_threads.tail = NULL;
}

//Semaphore down
void mutex_lock(struct mutex * i_mutex){
    i_mutex->held--;
    if(i_mutex->held < 0){
        current_thread->state = BLOCKED;
        thread_enqueue(&(i_mutex->waiting_threads), current_thread);
        yield();
    }
}

void mutex_unlock(struct mutex * i_mutex){
    i_mutex->held++;
    //if(!is_empty(&(i_mutex->waiting_threads))){
    if(i_mutex->held <= 0){
        if(is_empty(&(i_mutex->waiting_threads))){
            printf("Waiting threads queue in mutex is empty\n");
            exit(EXIT_FAILURE);
        }
        //Dequeue from wainting threads and enqueue into ready list
        struct thread* tmp_thread = thread_dequeue(&(i_mutex->waiting_threads));
        tmp_thread->state = READY;
        thread_enqueue(&ready_list, tmp_thread);
    }
}

void condition_init(struct condition * i_cond){
    i_cond->waiting_threads.head = NULL;
    i_cond->waiting_threads.tail = NULL;
}

void condition_wait(struct condition * i_cond, struct mutex * i_mutex){
    //Unlock mutex and wait
    mutex_unlock(i_mutex);
    //Wait
    current_thread->state = BLOCKED;
    thread_enqueue(&(i_cond->waiting_threads), current_thread);
    yield();
    //Relock mutex
    mutex_lock(i_mutex);
}

void condition_signal(struct condition * i_cond){
    if(!is_empty(&(i_cond->waiting_threads))){
        //Dequeue from wainting threads and enqueue into ready list
        struct thread* tmp_thread = thread_dequeue(&(i_cond->waiting_threads));
        tmp_thread->state = READY;
        thread_enqueue(&ready_list, tmp_thread);
    }
}

void condition_broadcast(struct condition * i_cond){
    while(!is_empty(&(i_cond->waiting_threads))){
        //Dequeue from wainting threads and enqueue into ready list
        struct thread* tmp_thread = thread_dequeue(&(i_cond->waiting_threads));
        tmp_thread->state = READY;
        thread_enqueue(&ready_list, tmp_thread);
    }
}

void thread_join(struct thread* i_thread){
    //Acquire thread's mutex
    //Do we really need to do this?
    mutex_lock(&(i_thread->t_mutex));

    //Wait until done
    while(i_thread->state != DONE){
        condition_wait(&(i_thread->t_cond), &(i_thread->t_mutex));
    }
    //TODO i_thread's memory can be freed here, but for simplicity, we wait until scheduler_end
    mutex_unlock(&(i_thread->t_mutex));
}

