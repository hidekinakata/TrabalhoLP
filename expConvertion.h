//
// Created by Hideki Nakata on 26/06/2018.
//

#ifndef SIMPLETRONC_EXPCONVERTION_H
#define SIMPLETRONC_EXPCONVERTION_H

#include <malloc.h>
#include <stdbool.h>

typedef struct _regPilha{
    char dado;
    struct regPilha *prox;
}RegPilha;

typedef RegPilha* PilhaChar;

RegPilha *alocaPilha();

PilhaChar CriaPilha();

void LiberaPilha(PilhaChar p);

bool PilhaVazia(PilhaChar p);

void Empilha(PilhaChar p, char x);

char Desempilha(PilhaChar p);

void In2Pos(char *infix, char *posfix);
int Prioridade(char c, char t);
#endif //SIMPLETRONC_EXPCONVERTION_H
