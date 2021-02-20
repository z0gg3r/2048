CC=gcc
CFLAGS=-Wall
RUSTFLAGS=-O -o scores
LIBS=-lncurses

OBJQ = board.o main.o

%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

2048: $(OBJQ)
	$(CC) -o $@ $^ $(CLFAGS) $(LIBS)
	rm -f *.o

clean:
	rm -f 2048 2048.exe SCORES *.o
