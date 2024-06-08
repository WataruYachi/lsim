#include <stdio.h>
#include <stdlib.h>
#include <gc.h>
#include "term.h"

//t = v
term var(char *x) {
    term t = GC_MALLOC(sizeof(struct term));
    t -> form = TERM_VAR;
    t -> name = x;
    t -> t1 = NULL;
    t -> t2 = NULL;
    return t;
}

// t = f(t1, t2)
term fun(char *f, term t1, term t2) {
    term t = GC_MALLOC(sizeof(struct term));
    t -> form = TERM_FUN;
    t -> name = f;
    t -> t1 = t1;
    t -> t2 = t2;
    return t;
}

// t = c
term con(char *c) {
    term t = GC_MALLOC(sizeof(struct term));
    t -> form = TERM_CON;
    t -> name = c;
    t -> t1 = NULL;
    t -> t2 = NULL;
    return t;
}

void print(term t) {
    if (t->form == TERM_VAR)
        printf("$%s", t -> name);
    else if (t -> form == TERM_FUN) {
        printf("%s", t -> name);
        printf("(");
        print(t -> t1);
        printf(", ");
        print(t -> t2);
        printf(")");
    } else if (t -> form == TERM_CON) {
      printf("%s", t -> name);
    } 
}

/*
int main(int argc, char *argv[]){
   term t = fun("f", var("x"), con("c"));
    print(t);
    return 0;
}
*/


