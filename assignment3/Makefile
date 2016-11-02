default: main

main: main.c scheduler.c queue.c switch.s async.c
	gcc -g -Wall -Wextra -Wpedantic main.c scheduler.c queue.c switch.s async.c -lrt
snake: 
	gcc -g -Wall -Wextra -Wpedantic scheduler.c queue.c switch.s async.c snake.c -lrt

