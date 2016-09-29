#include <stdio.h>
#include <stdlib.h>
#define STACK_SIZE  1024*1024

struct thread {
    //Stack pointer
    unsigned char* stack_pointer;
    //Initial function pointer that takes a void* as an argument
    void (*initial_function)(void*);
    //Initial argument
    void* initial_argument;
};

//Function prototype for threads 
void thread_switch(struct thread * old, struct thread * new);
void thread_start(struct thread * old, struct thread * new);

//Global pointer to current_thread
struct thread* current_thread;
struct thread* inactive_thread;

void yield() {
    struct thread * temp = current_thread;
    current_thread = inactive_thread;
    inactive_thread = temp;
    thread_switch(inactive_thread, current_thread);
}

void thread_wrap() {
    current_thread->initial_function(current_thread->initial_argument);
    yield();
}

//Test functions
int factorial (int n) {
    return n == 0? 1: n * factorial(n-1);
}

void fun_with_threads (void* arg){
    printf("In thread\n");
    int n = *(int*) arg;
    //for(int i = 0; i < 2; i++){
    //    printf("In thread\n");
    //    yield();
    //}
    printf("%d! = %d\n", n, factorial(n));
}


int main(void){
    //Allocate current_thread
    current_thread = (struct thread*) malloc(sizeof(struct thread));
    //Initialize current_thread
    current_thread->initial_function = fun_with_threads;
    int * p = malloc(sizeof(int));
    *p = 5;
    current_thread->initial_argument = p;
    current_thread->stack_pointer = malloc(STACK_SIZE) + STACK_SIZE;
    //current_thread->initial_function(current_thread->initial_argument);
    
    inactive_thread = (struct thread*) malloc(sizeof(struct thread));

    thread_start(inactive_thread, current_thread);

    printf("In main\n");

    //for(int i = 0; i < 2; i++){
    //    //This yield will fail on the 3rd try since the thread is done
    //    printf("In main\n");
    //    yield();
    //}

}
