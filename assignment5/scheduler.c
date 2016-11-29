#define _GNU_SOURCE
#include <sched.h>

#include <stdlib.h>
#include <stdio.h>
#define STACK_SIZE  1024*1024
#define CLONE_FLAGS CLONE_THREAD|CLONE_VM|CLONE_SIGHAND|CLONE_FILES|CLONE_FS|CLONE_IO

#include "scheduler.h"

#undef malloc
#undef free
void * safe_mem(int op, void * arg) {
  static AO_TS_t spinlock = AO_TS_INITIALIZER;
  void * result = 0;

  spinlock_lock(&spinlock);
  if(op == 0) {
	result = malloc((size_t)arg);
  } else {
	free(arg);
  }
  spinlock_unlock(&spinlock);
  return result;
}
#define malloc(arg) safe_mem(0, ((void*)(arg)))
#define free(arg) safe_mem(1, arg)

//Locks
AO_TS_t ready_list_lock = AO_TS_INITIALIZER;
AO_TS_t done_list_lock = AO_TS_INITIALIZER;
AO_TS_t threadCount_lock = AO_TS_INITIALIZER;
AO_TS_t printf_lock = AO_TS_INITIALIZER;

struct queue ready_list;
struct queue done_list;
//struct thread * current_thread;

//Function prototype for threads 
//Calling threads must be responsible for acquiring and releasing the ready_list_lock
void thread_switch(struct thread * old, struct thread * new);
void thread_start(struct thread * old, struct thread * new);


void thread_wrap() {
    spinlock_unlock(&ready_list_lock);
    current_thread->initial_function(current_thread->initial_argument);
    current_thread->state = DONE;
    //Signal thread conditional variable
    //condition_broadcast(&(current_thread->t_cond));
    yield();
}

int threadCount = 0;

//Clone requires return value
int kernel_thread_begin(){
    //Allocate current thread
    set_current_thread((struct thread*) malloc(sizeof(struct thread)));
    current_thread->state = RUNNING;
    //mutex_init(&(current_thread->t_mutex));
    //condition_init(&(current_thread->t_cond));
    //Yield forever
    while(1){
        yield();
    }
    return 0;//Should never get here
}

//One time setup and initialization
void scheduler_begin(){
    //Allocate current thread
    set_current_thread((struct thread*) malloc(sizeof(struct thread)));
    current_thread->state = RUNNING;

    spinlock_lock(&threadCount_lock);
    current_thread->threadId = threadCount;
    threadCount++;
    spinlock_unlock(&threadCount_lock);
    //mutex_init(&(current_thread->t_mutex));
    //condition_init(&(current_thread->t_cond));

    ready_list.head = NULL;
    ready_list.tail = NULL;
    done_list.head = NULL;
    done_list.tail = NULL;

    unsigned char * stack = (unsigned char*) malloc(STACK_SIZE) + STACK_SIZE; 
    //Create new thread
    clone(&kernel_thread_begin, stack, CLONE_FLAGS, NULL);
}

struct thread* thread_fork(void(*target)(void*), void * arg){
    //Allocate new thread control block and stack
    struct thread * new_thread = (struct thread*) malloc(sizeof(struct thread));
    new_thread->base_stack = (unsigned char*) malloc(STACK_SIZE);
    new_thread->stack_pointer = new_thread->base_stack + STACK_SIZE;
    new_thread->initial_function = target;
    new_thread->initial_argument = arg;
    //mutex_init(&(new_thread->t_mutex));
    //condition_init(&(new_thread->t_cond));

    spinlock_lock(&threadCount_lock);
    new_thread->threadId = threadCount;
    threadCount++;
    spinlock_unlock(&threadCount_lock);

    //Switching context
    current_thread->state = READY;
    //Acquire ready lock
    spinlock_lock(&ready_list_lock);
    thread_enqueue(&ready_list, current_thread);
    new_thread->state = RUNNING;
    struct thread * tmp_thread = current_thread;
    set_current_thread(new_thread);
    thread_start(tmp_thread, current_thread);
    spinlock_unlock(&ready_list_lock);
    return new_thread;
}

void yield() {
    spinlock_lock(&ready_list_lock);
    //Thread is runnable
    switch(current_thread->state){
        case RUNNING:
            current_thread->state = READY;
            thread_enqueue(&ready_list, current_thread);
            break;
        case DONE:
            spinlock_lock(&done_list_lock);
            thread_enqueue(&done_list, current_thread);
            spinlock_unlock(&done_list_lock);
            break;
        case READY:
            //current thread cannot be ready
            spinlock_lock(&printf_lock);
            printf("Error, current thread has READY state\n");
            spinlock_unlock(&printf_lock);
            exit(EXIT_FAILURE);
        case BLOCKED:
            //Does not enqueue into ready list
            break;
        default:
            spinlock_lock(&printf_lock);
            printf("Error, thread state %d not recognized\n", current_thread->state);
            spinlock_unlock(&printf_lock);
            exit(EXIT_FAILURE);
    }

    struct thread * tmp_thread = current_thread;

    //Make sure there are other threads to run
    //If no other threads, no need to context switch
    if(!is_empty(&ready_list)){
        set_current_thread(thread_dequeue(&ready_list));
        current_thread->state = RUNNING;
        //printf("Switching thread %d to thread %d\n", tmp_thread->threadId, current_thread->threadId);
        thread_switch(tmp_thread, current_thread);
    }
    spinlock_unlock(&ready_list_lock);
}

void scheduler_end(){
    //While the queue is not empty, we call yield
    spinlock_lock(&ready_list_lock);
    while(!is_empty(&ready_list)){
        spinlock_unlock(&ready_list_lock);
        yield();
        spinlock_lock(&ready_list_lock);
    }
    spinlock_unlock(&ready_list_lock);

    spinlock_lock(&done_list_lock);
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
    spinlock_unlock(&done_list_lock);

    //Free current thread block (main thread)
    free(current_thread);
}

//void mutex_init(struct mutex * i_mutex){
//    i_mutex->held = 1;
//    i_mutex->waiting_threads.head = NULL;
//    i_mutex->waiting_threads.tail = NULL;
//}
//
////Semaphore down
//void mutex_lock(struct mutex * i_mutex){
//    i_mutex->held--;
//    if(i_mutex->held < 0){
//        current_thread->state = BLOCKED;
//        thread_enqueue(&(i_mutex->waiting_threads), current_thread);
//        yield();
//    }
//}
//
//void mutex_unlock(struct mutex * i_mutex){
//    i_mutex->held++;
//    //if(!is_empty(&(i_mutex->waiting_threads))){
//    if(i_mutex->held <= 0){
//        if(is_empty(&(i_mutex->waiting_threads))){
//            printf("Waiting threads queue in mutex is empty\n");
//            exit(EXIT_FAILURE);
//        }
//        //Dequeue from wainting threads and enqueue into ready list
//        struct thread* tmp_thread = thread_dequeue(&(i_mutex->waiting_threads));
//        tmp_thread->state = READY;
//        thread_enqueue(&ready_list, tmp_thread);
//    }
//}
//
//void condition_init(struct condition * i_cond){
//    i_cond->waiting_threads.head = NULL;
//    i_cond->waiting_threads.tail = NULL;
//}
//
//void condition_wait(struct condition * i_cond, struct mutex * i_mutex){
//    //Unlock mutex and wait
//    mutex_unlock(i_mutex);
//    //Wait
//    current_thread->state = BLOCKED;
//    thread_enqueue(&(i_cond->waiting_threads), current_thread);
//    yield();
//    //Relock mutex
//    mutex_lock(i_mutex);
//}
//
//void condition_signal(struct condition * i_cond){
//    if(!is_empty(&(i_cond->waiting_threads))){
//        //Dequeue from wainting threads and enqueue into ready list
//        struct thread* tmp_thread = thread_dequeue(&(i_cond->waiting_threads));
//        tmp_thread->state = READY;
//        thread_enqueue(&ready_list, tmp_thread);
//    }
//}
//
//void condition_broadcast(struct condition * i_cond){
//    while(!is_empty(&(i_cond->waiting_threads))){
//        //Dequeue from wainting threads and enqueue into ready list
//        struct thread* tmp_thread = thread_dequeue(&(i_cond->waiting_threads));
//        tmp_thread->state = READY;
//        thread_enqueue(&ready_list, tmp_thread);
//    }
//}
//
//void thread_join(struct thread* i_thread){
//    //Acquire thread's mutex
//    mutex_lock(&(i_thread->t_mutex));
//
//    //Wait until done
//    while(i_thread->state != DONE){
//        condition_wait(&(i_thread->t_cond), &(i_thread->t_mutex));
//    }
//    //TODO i_thread's memory can be freed here, but for simplicity, we wait until scheduler_end
//    mutex_unlock(&(i_thread->t_mutex));
//}

void spinlock_lock(AO_TS_t * inLock){
    //Loop until cleared
    while(AO_test_and_set_acquire(inLock) != AO_TS_CLEAR){};
}
void spinlock_unlock(AO_TS_t * inLock){
    AO_CLEAR(inLock);
}

