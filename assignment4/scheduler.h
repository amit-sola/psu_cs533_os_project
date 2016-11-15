#include <sys/types.h>
#include "queue.h"

typedef enum{
    RUNNING, //Thre thread is currently running
    READY, //The thread is not running but runnable
    BLOCKED, //The thread is not running, and is not runnable
    DONE //The thread has finished
}state_t;

struct mutex{
    int held;
    struct queue waiting_threads;
};

struct condition{
    struct queue waiting_threads;
};

struct thread {
    //Stack pointer
    unsigned char* stack_pointer;
    //Initial function pointer that takes a void* as an argument
    void (*initial_function)(void*);
    //Initial argument
    void* initial_argument;
    state_t state;
    unsigned char* base_stack;
    struct mutex t_mutex;
    struct condition t_cond;
    int threadId;
};

//One time setup and initialization
void scheduler_begin();
//Switches among runnable threads
struct thread* thread_fork(void(*target)(void*), void * arg);
//Switches among runnable threads
void yield();
//To wait for all thraeds in the system to finish
void scheduler_end();
//Joins all threads
void thread_join(struct thread*);

ssize_t read_wrap(int fd, void* buf, size_t count);

extern struct thread * current_thread;


void mutex_init(struct mutex *);
void mutex_lock(struct mutex *);
void mutex_unlock(struct mutex *);

void condition_init(struct condition *);
void condition_wait(struct condition *, struct mutex *);
void condition_signal(struct condition *);
void condition_broadcast(struct condition *);




