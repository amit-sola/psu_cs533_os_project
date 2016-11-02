#include "scheduler.h"
#include <stdio.h>

void print_nth_prime(void * pn) {
  int n = *(int *) pn;
  int c = 1, i = 1;
  while(c <= n) {
    ++i;
    int j, isprime = 1;
    for(j = 2; j < i; ++j) {
      if(i % j == 0) {
        isprime = 0;
        break;
      }
    }
    if(isprime) {
      ++c;
    }
    yield();
  }
  printf("%dth prime: %d\n", n, i);
 
}

void print_n(void * n){
    yield();
    int nn = *(int * ) n;
    printf("%d\n", nn);
}

void echo(void * n){
    char buf[100];
    //0 is STDIN_FILENO
    read_wrap(0, buf, 99);
    printf("Echoing: %s", buf);
}


int main(void) {
  scheduler_begin();

  int n1 = 20000, n2 = 10000, n3 = 30000;
  thread_fork(print_nth_prime, &n1);
  thread_fork(print_nth_prime, &n2);
  thread_fork(print_nth_prime, &n3);
  thread_fork(echo, &n1);

  scheduler_end();
}


