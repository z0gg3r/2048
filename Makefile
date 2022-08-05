CC=gcc
CFLAGS=-Wall -Wextra -std=c99 \
       -pipe -march=native -O2 \
       -D_FORTIFY_SOURCE=2 \
       -fstack-protector-strong \
       -fcf-protection
LIBS=-lncurses -ltinfo
PREFIX=/usr/local/bin
SCORES=/usr/local/share/2048_scores.sh

OBJQ = board.o main.o

%.o: src/%.c
	@echo CC $^
	@$(CC) -c -o $@ $< $(CFLAGS)

2048: $(OBJQ)
	@echo CC $^
	@$(CC) -o $@ $^ $(CLFAGS) $(LIBS)
	@rm -f *.o

clean:
	@echo Cleaning dir...
	@rm -f 2048 2048.exe SCORES *.o

install:
	@echo INSTALL 2048
	@cp 2048 $(PREFIX)
	@echo INSTALL scores.sh
	@cp scores.sh $(SCORES)
