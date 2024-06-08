#ifndef TERM_H
#define TERM_H

/* term  t ::= VAR(ID) | FUN(ID, t0, t1) | CON(ID)
   where ID : char * */

typedef enum { TERM_VAR, TERM_FUN, TERM_CON } form;

struct term{
    form form;
    char *name;
    struct term *t1;
    struct term *t2;
};

typedef struct term *term;

term var(char *);
term fun(char *, term, term);
term con(char *);
void print(term);

#endif
