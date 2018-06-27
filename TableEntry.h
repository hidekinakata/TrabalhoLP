//
// Created by Hideki Nakata on 26/06/2018.
//

#ifndef _TABLEENTRY_H
#define _TABLEENTRY_H

#define MAX 100

#define LINE 'L'
#define CONSTC 'C'
#define VAR 'V'

typedef struct entry TableEntry;
struct entry {
    int symbol;
    char type;
    int location;
};

void iniciaTabela(TableEntry *tabela);
int busca(TableEntry *tabela, int simbolo, char tipo);
void insereVAR( TableEntry *symbolTable, int data, int *contadorDados, int *contadorTabela );
void InsereCONST( TableEntry *symbolTable, int data, int *contadorDados, int *contadorTabela );
void insereLINE( TableEntry *symbolTable, int data, int *contadorInstrucao, int *contadorTabela );




#endif //SIMPLETRONC_TABLEENTRY_H
