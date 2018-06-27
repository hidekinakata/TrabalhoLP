//
// Created by Hideki Nakata on 26/06/2018.
//

#ifndef SIMPLETRONC_PILHAENDERECOS_H
#define SIMPLETRONC_PILHAENDERECOS_H

typedef struct localStack{
    int local;
    struct localStack *prox;
}StackReg;

typedef StackReg* Stack;

StackReg *allocREG();

Stack criaStack();

void LiberaStack(Stack p);

bool StackVazia(Stack p);

void push(Stack p, int x);

int pop(Stack p);


#endif //SIMPLETRONC_PILHAENDERECOS_H
