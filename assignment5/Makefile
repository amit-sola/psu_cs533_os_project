default: test3


test0: counter_test.c scheduler.c queue.c switch.s async.c threadmap.c
	gcc -g -Wall -Wextra -Wpedantic counter_test.c scheduler.c queue.c switch.s async.c threadmap.c -lrt -o test0

test1: counter_test.c scheduler.c queue.c switch.s async.c threadmap.c
	gcc -g -Wall -Wextra -Wpedantic counter_test.c scheduler.c queue.c switch.s async.c threadmap.c -lrt -o test1

test2: spinlock_test.c
	gcc -I ~/local/include -g -Wall -Wextra -Wpedantic spinlock_test.c -o test2

test3: factorial_test.c scheduler.c queue.c switch.s async.c threadmap.c
	gcc -I ~/local/include -g -Wall -Wextra -Wpedantic factorial_test.c scheduler.c queue.c switch.s async.c threadmap.c -lrt -o test3

#main: main.c scheduler.c queue.c switch.s async.c
#	gcc -g -Wall -Wextra -Wpedantic main.c scheduler.c queue.c switch.s async.c -lrt -o main
#demo1: demo1.c scheduler.c queue.c switch.s async.c 
#	gcc -g -Wall -Wextra -Wpedantic demo1.c scheduler.c queue.c switch.s async.c -lrt -o demo1
#demo2: demo2.c scheduler.c queue.c switch.s async.c 
#	gcc -g -Wall -Wextra -Wpedantic demo2.c scheduler.c queue.c switch.s async.c -lrt -o demo2
#sort_test: sort_test.c scheduler.c queue.c switch.s async.c 
#	gcc -g -Wall -Wextra -Wpedantic sort_test.c scheduler.c queue.c switch.s async.c -lrt -o sort_test


clean:
	rm -f test0 test1 test2 test3

