CC=gcc
CFLAGS=-O

all: msg_sender msg_receiver msg_private

ex_% : ex_%.o
	$(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f *.o msg_sender msg_receiver msg_private
