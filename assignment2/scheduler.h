
typedef enum{
    RUNNING, //Thre thread is currently running
    READY, //The thread is not running but runnable
    BLOCKED, //The thread is not running, and is not runnable
    DONE //The thread has finished
}state_t;

struct thread {
    //Stack pointer
    unsigned char* stack_pointer;
    //Initial function pointer that takes a void* as an argument
    void (*initial_function)(void*);
    //Initial argument
    void* initial_argument;
    state_t state;
    int threadId;
};

//One time setup and initialization
void scheduler_begin();
//Switches among runnable threads
void thread_fork(void(*target)(void*), void * arg);
//Switches among runnable threads
void yield();
//To wait for all thraeds in the system to finish
void scheduler_end();

extern struct thread * current_thread;

