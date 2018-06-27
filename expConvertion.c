//
// Created by Hideki Nakata on 26/06/2018.
//

#include "expConvertion.h"
#include <string.h>

void In2Pos(char *infix, char *posfix){
    PilhaChar p;
    int i = 0;
    char c,t;
    char aux[2];
    aux[1] = '\0';
    posfix[0] = '\0'; // 'reseta' a string
    p = CriaPilha();
    Empilha(p, '(');

    do{
        c = infix[i];
        i++;
        if(c >= 'a' && c <= 'z' || (c >= '0' && c <= '9')){
            aux[0] = c;
            strcat(posfix, aux);
        }
        else if(c == '('){
            Empilha(p, '(');
        }
        else if(c == ')' || c == '\0'){
            do{
                t = Desempilha(p);
                if(t != '('){
                    aux[0] = t;
                    strcat(posfix, aux);
                }
            }while(t != '(');
        }
        else if(c == '+' || c == '-' ||
                c == '*' || c == '/'  ){
            while(1){
                t = Desempilha(p);
                if(Prioridade(c,t)){
                    Empilha(p, t);
                    Empilha(p, c);
                    break;
                }
                else{
                    aux[0] = t;
                    strcat(posfix, aux);
                }
            }
        }
    }while(c != '\0');
    LiberaPilha(p);
}

int Prioridade(char c, char t){
    int pc,pt;

    if(c == '*' || c == '/')
        pc = 2;
    else if(c == '+' || c == '-')
        pc = 1;
    else if(c == '(')
        pc = 2;

    if(t == '*' || t == '/')
        pt = 2;
    else if(t == '+' || t == '-')
        pt = 1;
    else if(t == '(')
        pt = 0;

    return (pc > pt);
}

RegPilha *alocaPilha(){
    RegPilha* q;
    q = (RegPilha*) malloc(sizeof(RegPilha));
    if(q==NULL) exit(-1);
    return q;
}

PilhaChar CriaPilha(){
    PilhaChar p;
    p = alocaPilha();
    p->prox = NULL;
    return p;
}

void LiberaPilha(PilhaChar p){
    RegPilha *q,*t;
    q = p;
    while(q!=NULL){
        t = q;
        q = q->prox;
        free(t);
    }
}

bool PilhaVazia(PilhaChar p){
    return (p->prox==NULL);
}

void Empilha(PilhaChar p, char x){
    RegPilha *q;
    q = alocaPilha();
    q->dado = x;
    q->prox = p->prox;
    p->prox = q;
}

char Desempilha(PilhaChar p){
    RegPilha *q;
    char x;
    q = p->prox;
    if(q==NULL) exit(-1);
    x = q->dado;
    p->prox = q->prox;
    free(q);
    return x;
}