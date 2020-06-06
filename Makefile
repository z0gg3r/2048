RSC=rustc
CC=gcc
RSFLAGS=-O2 --crate-type=dylib
CFLAGS=-Wall
RUSTFLAGS=-O -o bin/scores
LIBS=-lncurses

OBJQ = board.o main.o

%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

2048: $(OBJQ)
	$(CC) -o bin/$@ $^ $(CLFAGS) $(LIBS)
	$(RSC) $(RUSTFLAGS) src/scores.rs
	rm -f *.o

clean:
	rm -f bin/2048 bin/2048.exe bin/scores *.o
