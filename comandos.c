//
// Created by Hideki Nakata on 26/06/2018.
//

#include "comandos.h"


/*
 * Verifica a validade e classifica a entrada de comando
 */
int tipoComando(char *string){
    if (!strcmp(string, "rem") || !strcmp(string, "rem\n")) {
        return rem;
    }

    if (!strcmp(string, "input"))
        return input;

    if (!strcmp(string, "let"))
        return let;

    if (!strcmp(string, "print"))
        return print;

    if (!strcmp(string, "goto"))
        return goto_;

    if (!strcmp(string, "if"))
        return if_;

    if (!strcmp(string, "end\n") || !strcmp(string, "end"))
        return end;

    return -1;
}

/*
 * Verifica se um char* é completamente numerico
 */
int isNum(char *num){
    int i = 0;
    for (i = 0; i < strlen(num); ++i) {
        if ((num[i] >= '0' && num[i] <= '9') == 0)
            return 0;
    }
    return 1;
}

// Verifica se é um uma varievel válida
// ou seja, se esta entre 'a' e z'  e é unica
int isVar(char *var){
    if (strlen(var) == 1)
        return *var >= 'a' && *var <= 'z';
    return 0;
}

/*
 *  verificação não tão rigorosa de expressão valida
 *  Verifica se são char numericos, operadores, variaveis,
 *  parenteses(e se os parenteses fecham)
 *
 */
int isValidExp(char *exp){
    int i;
    int lparen = 0, rparen = 0;
    for (i = 0; i < strlen(exp); ++i) {
        if (exp[i] == '(') lparen++;
        else if (exp[i] == ')') rparen++;
        else if ((exp[i] >= '0' && exp[i] <= '9') ||
                exp[i] == '+' ||
                exp[i] == '-' ||
                exp[i] == '*' ||
                exp[i] == '/');
        else{
            return 0;
        }
    }
    return lparen == rparen;
}

int isOp(char *var){
    if (strlen(var) == 1){
        return ( *var == '+' || *var == '-' || *var == '/' || *var == '*');
    }
    return 0;
}

int operador( char c ) {
    if     ( c == '+' ) return ADD;
    else if( c == '-' ) return SUB;
    else if( c == '*' ) return MUL;
    else if( c == '/' ) return DIV;
}