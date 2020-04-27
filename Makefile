all: build

CC=clang
STD=gnu99
OPT=-Os
DBG=-g -ggdb
OTHER=

CMD=$(CC) $(DBG) -std=$(STD) $(OPT) $(OPT)
LINK=-lncurses

FINBIN=bin/matrix

install: build ;
	cp -u $(FINBIN) /usr/local/bin/

build:
	$(CMD) ./src/main.c $(LINK) -o ./bin/matrix

valgrind: build ;
	valgrind ./$(FINBIN)

clean:
	rm -rf ./bin/*

clean-logs:
	rm ./vgcore.*
	rm ./massif.out.*
