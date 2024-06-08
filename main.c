#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>
#include <gc.h>
#include "gate.h"
#include "circuit.h"
#include "truthTable.h"
#include "qm.h"

circuit halfadder() {
    char *inputIds[] = {"x", "y"};
    gate* input = inputArray(2, inputIds);
    gate and1 = And("and1", 2, input);
    gate not1 = Not("not1", and1);
    gate or1  = Or("or1",2, input);
    gate *and2input = gateArray(2);
    and2input[0] = not1;
    and2input[1] = or1;
    gate and2 = And("and2", 2, and2input);
    gate outc = Output("c", and1);
    gate outs = Output("s", and2);
    gate* output = gateArray(2);
    output[0] = outc;
    output[1] = outs;
    return Circuit("half adder", 2, input, 2, output);
}


circuit andc() {
    gate *input = gateArray(2);
    input[0] = Input("x");
    input[1] = Input("y");
    gate and1 = And("and1", 2, input);
    gate out = Output("z", and1);
    gate *output = gateArray(1);
    output[0] = out;
    return Circuit("and", 2, input, 1, output);
}

circuit major() {
    char *inputVars[] = {"x", "y", "z"};
    gate *input = inputArray(3, inputVars);
    gate not1 = Not("notX", input[0]);
    gate not2 = Not("notY", input[1]);
    gate not3 = Not("notZ", input[2]);
    gate *and1input = gateArray(3);
    and1input[0] = not1;
    and1input[1] = input[1];
    and1input[2] = input[2];
    gate and1 = And("and1", 3, and1input);
    gate *and2input = gateArray(3);
    and2input[0] = input[0];
    and2input[1] = not2;
    and2input[2] = input[2];
    gate and2 = And("and2", 3, and2input);
    gate *and3input = gateArray(3);
    and3input[0] = input[0];
    and3input[1] = input[1];
    and3input[2] = not3;
    gate and3 = And("and3", 3, and3input);
    gate *and4input = gateArray(3);
    and4input[0] = input[0];
    and4input[1] = input[1];
    and4input[2] = input[2];
    gate and4 = And("and4", 3, and4input);
    gate *orinput = gateArray(4);
    orinput[0] = and1;
    orinput[1] = and2;
    orinput[2] = and3;
    orinput[3] = and4;
    gate or1 = Or("or1", 4, orinput); 
    gate *output = gateArray(1);
    output[0] = Output("m", or1);
    return Circuit("major", 3, input, 1, output);  
}

gate and1() {
    gate *input = gateArray(2);
    input[0] = Input("x");
    input[1] = Input("y");
    setGateValue(input[0],1);
    setGateValue(input[1],0);
    gate and1 = And("and", 2, input);
    return and1; 
}

int main(int argc, char const *argv[]) {
    circuit ha = halfadder();
    circuit aa = andc();
    int input[2] = {1, 1};
    truthTable tt = makeTruthTable(ha);
    printTruthTable(tt);
    int *r = runCircuitWithInput(ha, 2, input);
    //printf("%d,%d,%d,%d\n", r[0],r[1],r[2],r[3]);
    truthTable mtt = makeTruthTable(major());
    printTruthTable(mtt);
    //qm(major());
    qmsample(major());
    return 0;
}
