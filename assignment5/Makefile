default: main


test0: counter_test.c scheduler.c queue.c switch.s async.c 
	gcc -g -Wall -Wextra -Wpedantic counter_test.c scheduler.c queue.c switch.s async.c threadmap.c -lrt -o test0

#main: main.c scheduler.c queue.c switch.s async.c
#	gcc -g -Wall -Wextra -Wpedantic main.c scheduler.c queue.c switch.s async.c -lrt -o main
#demo1: demo1.c scheduler.c queue.c switch.s async.c 
#	gcc -g -Wall -Wextra -Wpedantic demo1.c scheduler.c queue.c switch.s async.c -lrt -o demo1
#demo2: demo2.c scheduler.c queue.c switch.s async.c 
#	gcc -g -Wall -Wextra -Wpedantic demo2.c scheduler.c queue.c switch.s async.c -lrt -o demo2
#sort_test: sort_test.c scheduler.c queue.c switch.s async.c 
#	gcc -g -Wall -Wextra -Wpedantic sort_test.c scheduler.c queue.c switch.s async.c -lrt -o sort_test


clean:
	rm -f test0

