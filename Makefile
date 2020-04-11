CC=gcc
CFLAGS=
LIBS=

OBJQ = board.o main.o

%.o: src/%.c
	$(CC) -c -o bin/$@ $< $(CFLAGS)

2048: $(OBJQ)
	$(CC) -o bin/$@ bin/$^ $(CLFAGS) $(LIBS)

clean:
	rm -f bin/*
