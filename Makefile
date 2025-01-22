CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

main: main.o prio_queue.o utils.o
	$(CC) $(CFLAGS) -o main main.o prio_queue.o utils.o $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

prio_queue.o: prio_queue.c
	$(CC) $(CFLAGS) -c -o prio_queue.o prio_queue.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c -o utils.o utils.c

clean:
	rm -f *.0 main