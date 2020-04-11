CFLAGS=
LIBS=

OBJQ = board.o

%.o: src/%.c
	gcc -c -o bin/$@ $< $(CFLAGS)

2048: $(OBJQ)
	gcc -o bin/$@ bin/$^ $(CLFAGS) $(LIBS)

clean:
	rm -f bin/*
