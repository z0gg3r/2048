2048: src/l2048.c src/game.c
	gcc src/l2048.c src/game.c -o bin/2048.exe

clean:
	rm -f bin/2048.exe

.PHONY: clean
