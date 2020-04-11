CC=gcc
CFLAGS=
LIBS=

OBJQ = bin/board.o bin/main.o

%.o: src/%.c
	$(CC) -c -o bin/$@ $< $(CFLAGS)

2048: $(OBJQ)
	$(CC) -o bin/$@ $^ $(CLFAGS) $(LIBS)

clean:
	rm -f bin/*
