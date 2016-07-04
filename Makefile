all: tracker

tracker: tracker.c
	gcc -o tracker tracker.c

clean:
	rm -f tracker output *.o *~ core

clear:
	rm -f output
	touch output


