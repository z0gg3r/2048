CC=gcc
CFLAGS=
LIBS=-lncurses

OBJQ = board.o main.o

%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

2048: $(OBJQ)
	$(CC) -o bin/$@ $^ $(CLFAGS) $(LIBS)

clean:
	rm -f bin/2048 bin/2048.exe *.o
