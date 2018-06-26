//
// Created by Hideki Nakata on 26/06/2018.
//

#ifndef SIMPLETRONC_COMANDOS_H
#define SIMPLETRONC_COMANDOS_H

#include <string.h>

#define READ 1000       //Lê uma palavra do teclado para uma posição específica da memória.
#define WRITE 1100      //Escreve na tela uma palavra de uma posição específica da memória.
#define LOAD 2000       //Carrega uma palavra de uma posição específica na memória para o acumulador.
#define STORE 2100      //Armazena uma palavra do acumulador em uma posição específica na memória.
#define ADD 3000        //Adiciona uma palavra de uma posição específica na memória à palavra no acumulador (deixa o resultado no acumulador).
#define SUB 3100        //Subtrai uma palavra de uma posição específica na memória à palavra no acumulador (deixa o resultado no acumulador).
#define DIV 3200        //Divide a palavra que está no acumulador por uma palavra de uma posição específica na memória (deixa o resultado no acumulador).
#define MUL 3300        //Multiplica uma palavra de uma posição específica na memória pela palavra no acumulador (deixa o resultado no acumulador).
#define BRANCH 4000     //Desvia para uma posição específica na memória.
#define BRANCHNEG 4100  //Desvia para uma posição específica na memória se o acumulador for negativo.
#define BRANCHZERO 4200 //Desvia para uma posição específica na memória se o acumulador for zero.
#define HALT 4300       //Finaliza o programa.

typedef enum{rem, input, let, print, goto_, if_, end} comandos;

int tipoComando(char *string);
int isNum(char *num);
int isVar(char *var);
int isOp(char *var);
int isValidExp(char *exp);
int operador( char c );
#endif //SIMPLETRONC_COMANDOS_H
