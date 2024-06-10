#ifndef BILDER_H
#define BILDER_H

#include "circuit.h"
#include "gate.h"
#include "ast.h"

struct assignment {
    char *var;
    gate g;
};

typedef struct assignment *assignment;

struct env {
    int n;
    assignment* a;
};

typedef struct env *env;

assignment Assignment(char *v, gate g);
env newEnv();
void addPair(env, assignment);

circuit circuitBuilder(AST node);
gate gateBuilder(env e, AST node);

#endif
