//
// Created by Hideki Nakata on 26/06/2018.
//

#include <malloc.h>
#include <stdbool.h>
#include "PilhaEnderecos.h"

StackReg *allocREG(){
    StackReg* q;
    q = (StackReg*) malloc(sizeof(StackReg));
    if(q==NULL) exit(-1);
    return q;
}

Stack criaStack(){
    Stack p;
    p = allocREG();
    p->prox = NULL;
    return p;
}

void LiberaPilha(Stack p){
    StackReg *q,*t;
    q = p;
    while(q!=NULL){
        t = q;
        q = q->prox;
        free(t);
    }
}

bool PilhaVazia(Stack p){
    return (p->prox==NULL);
}

void push(Stack p, int x){
    StackReg *q;
    q = allocREG();
    q->local = x;
    q->prox = p->prox;
    p->prox = q;
}

int pop(Stack p){
    StackReg *q;
    int x;
    q = (StackReg *) p->prox;
    if(q==NULL) exit(-1);
    x = q->local;
    p->prox = q->prox;
    free(q);
    return x;
}