all: build

CFLAGS=-lncurses

install: build ;
	cp -u $(FINBIN) /usr/local/bin/

build: ./matrix

./matrix: ./matrix.c

clean:
	rm -f ./matrix

run: bin/matrix ;
	./bin/matrix --tickTime 15 --streamLen 20
