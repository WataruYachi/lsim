#CC=gcc -std=c99
CC=cc -I/app/gc-7.1/include
FLEX=flex
BISON=bison
LDLIBS=-lfl -lgc -lm
PROGRAM=lc

all: $(PROGRAM)

%.o: %.c %.h
	$(CC) -c $<

$(PROGRAM): parse.y lex.l ast.o bit.o table.o gate.o circuit.o qm.o builder.o
	$(FLEX) lex.l
	$(BISON) -d parse.y
	$(CC) -g3 -o $@ bit.o gate.o table.o ast.o circuit.o qm.o builder.o lex.yy.c parse.tab.c $(LDLIBS)

clean:
	rm -f *.o *.yy.c *.tab.h *.tab.c $(PROGRAM)
