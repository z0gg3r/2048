CC=gcc
CFLAGS=-Wall
LIBS=-lncurses
PREFIX=/usr/local/bin
SCORES=/usr/local/share/2048_scores.sh

OBJQ = board.o main.o

%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

2048: $(OBJQ)
	$(CC) -o $@ $^ $(CLFAGS) $(LIBS)
	rm -f *.o

clean:
	rm -f 2048 2048.exe SCORES *.o

install:
	cp 2048 $(PREFIX)
	cp scores.sh $(SCORES)
