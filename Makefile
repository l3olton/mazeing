CC = gcc
CFLAGS = -Wall
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

mazeing: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

debug: main.c
	$(CC) $(CFLAGS) -g -o $@ $< $(LDLIBS)

clean:
	rm -f mazeing
