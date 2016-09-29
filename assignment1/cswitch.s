#void thread_switch(struct thread * old, struct thread * new);

.globl thread_switch
.globl thread_start

thread_switch:
    #Push all callee-save registers onto current stack
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    #Save current stack pointer in old's thread control block
    #%rsp is the current stack pointer
    #%rdi is first argument
    #stack_pointer is first field of struct
    movq %rsp, (%rdi)

    #Set new thread's stack pointer to current stack pointer
    #%rsi is the second argument
    movq (%rsi), %rsp

    #Pop all callee-save registers from new stack
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    #Return
    ret

#void thread_start(struct thread * old, struct thread * new);
thread_start:
    #Push all callee-save registers onto current stack
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    #Save current stack pointer in old's thread control block
    #%rsp is the current stack pointer
    #%rdi is first argument
    #stack_pointer is first field of struct
    movq %rsp, (%rdi)

    #Set new thread's stack pointer to current stack pointer
    #%rsi is the second argument
    movq (%rsi), %rsp

    #Call initial function of new
    jmp thread_wrap

    #Return
    ret

