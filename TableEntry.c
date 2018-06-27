//
// Created by Hideki Nakata on 26/06/2018.
//

#include "TableEntry.h"

void iniciaTabela(TableEntry *tabela){
    int i;
    tabela[0].symbol = 0;
    tabela[0].location = 0;
    for (i = 1; i < MAX; ++i) {
        tabela[i].symbol = -9999;
    }
}

//retona a posição do simbolo, se achar
int busca(TableEntry *tabela, int simbolo, char tipo){
    int i;
    for (i = 1; i < MAX; i++) {
        if (tabela[i].symbol == simbolo && tabela[i].type == tipo)
            return i;
    }
    return 0;
}

void insereVAR( TableEntry *symbolTable, int data, int *contadorDados, int *contadorTabela ) {
    symbolTable[ *contadorTabela ].symbol = data;
    symbolTable[ *contadorTabela ].type = 'V';
    symbolTable[ (*contadorTabela)++ ].location = (*contadorDados)--;
}

void InsereCONST( TableEntry *symbolTable, int data, int *contadorDados, int *contadorTabela ) {
    symbolTable[ *contadorTabela ].symbol = data;
    symbolTable[ *contadorTabela ].type = 'C';
    symbolTable[ (*contadorTabela)++ ].location = (*contadorDados)--;
}

void insereLINE( TableEntry *symbolTable, int data, int *contadorInstrucao, int *contadorTabela ) {
    symbolTable[ *contadorTabela ].symbol = data;
    symbolTable[ *contadorTabela ].type = 'L';
    symbolTable[ (*contadorTabela)++ ].location = *contadorInstrucao;
}