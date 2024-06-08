#CC=gcc -std=c99
CC=cc -I/app/gc-7.1/include
FLEX=flex
BISON=bison
LDLIBS=-lfl -lgc
PROGRAM=lc

all: $(PROGRAM)

%.o: %.c %.h
	$(CC) -c $<

$(PROGRAM): parse.y lex.l term.o
	$(FLEX) lex.l
	$(BISON) -d parse.y
	$(CC) -o $@ term.o lex.yy.c parse.tab.c $(LDLIBS)

clean:
	rm -f *.o *.yy.c *.tab.h *.tab.c $(PROGRAM)
