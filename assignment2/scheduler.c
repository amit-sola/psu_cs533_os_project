#include "scheduler.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#define STACK_SIZE  1024*1024

struct queue ready_list;
struct thread * current_thread;

//Function prototype for threads 
void thread_switch(struct thread * old, struct thread * new);
void thread_start(struct thread * old, struct thread * new);


void thread_wrap() {
    current_thread->initial_function(current_thread->initial_argument);
    current_thread->state = DONE;
    yield();
}

int threadCount = 0;

//One time setup and initialization
void scheduler_begin(){
    //Allocate current thread
    current_thread = (struct thread*) malloc(sizeof(struct thread));
    current_thread->state = RUNNING;
    current_thread->threadId = threadCount;
    threadCount++;
    ready_list.head = NULL;
    ready_list.tail = NULL;
}

void thread_fork(void(*target)(void*), void * arg){
    //Allocate new thread control block and stack
    struct thread * new_thread = (struct thread*) malloc(sizeof(struct thread));
    new_thread->stack_pointer = ((unsigned char*) malloc(STACK_SIZE)) + STACK_SIZE;
    new_thread->initial_function = target;
    new_thread->initial_argument = arg;
    new_thread->threadId = threadCount;
    threadCount++;
    //Switching context
    current_thread->state = READY;
    thread_enqueue(&ready_list, current_thread);
    new_thread->state = RUNNING;
    struct thread * tmp_thread = current_thread;
    current_thread = new_thread;
    thread_start(tmp_thread, current_thread);
}

void yield() {
    unsigned int deleteThread = 0;
    if(current_thread->state != DONE){
        current_thread->state = READY;
        thread_enqueue(&ready_list, current_thread);
    }
    else{
        deleteThread = 1;
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

    //Now that threads have switched, we check if the previous current thread can be freed
    if(deleteThread == 1){
        //Free allocated stack
        free(tmp_thread->stack_pointer);
        tmp_thread->stack_pointer = NULL;
        free(tmp_thread);
        tmp_thread= NULL;
    }
}

void scheduler_end(){
    //While the queue is not empty, we call yield
    while(!is_empty(&ready_list)){
        yield();
    }
}






