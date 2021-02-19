RSC=rustc
CC=gcc
RSFLAGS=-O2 --crate-type=dylib
CFLAGS=-Wall
RUSTFLAGS=-O -o scores
LIBS=-lncurses

OBJQ = board.o main.o

%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

2048: $(OBJQ)
	$(CC) -o $@ $^ $(CLFAGS) $(LIBS)
	$(RSC) $(RUSTFLAGS) src/scores.rs
	touch SCORES
	strip scores
	rm -f *.o

clean:
	rm -f 2048 2048.exe scores SCORES *.o
