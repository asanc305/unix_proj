all: tracker

tracker: tracker.c
	gcc -o tracker tracker.c

clean:
	rm -f tracker *.txt *.o *~ core


