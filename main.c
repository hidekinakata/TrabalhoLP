#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "TableEntry.h"
#include "comandos.h"
#include "expConvertion.h"
#include "PilhaEnderecos.h"

int main() {
    TableEntry tabela[MAX];
    int flags[100];
    int LM[100] = { 0 }; // Linguagem de Maquina, instruçoes;
    int contadorDados = 99;
    int contadorInstrucao = 0;
    int contadorTabela = 1;
    int *contadorInstrucaoPtr = &contadorInstrucao;
    int *contadorDadosPtr = &contadorDados;
    int *contadorTabelaPtr = &contadorTabela;

    char linha[1000];
    char *lexema;
    char instrucao[10];
    int linhaAtual;
    int i;

    // Para uso do 'let'
    char infix[50] = { 0 };
    char postfix[50] = { 0 };
    int store;
    Stack stack = criaStack();

    char nome[100];
    FILE *codigoFonte = NULL, *smlOUT;

    // Abrindo arquivo
    while (codigoFonte == NULL){
        system("cls");
        printf("\n\n\tDigite o nome do Codigo-fonte: ");
        gets(nome);
        system("cls");
        if ((codigoFonte = fopen(nome, "r")) == NULL){
            printf("\n\n\tErro ao abrir arquivo.");
        }
        else{
            printf("\n\n\tSucesso, Iniciando compilacao");
        }
        Sleep(1000);
        system("cls");
    }

    //  Primeira passagem //
    while(fgets(linha, 1000, codigoFonte)) {

        // primeiro lexema deve ser um numero de linha válido
        lexema = strtok(linha, " ");
        if (isNum(lexema) && (atoi(lexema) >= 0 && atoi(lexema) <= 99) && (busca(tabela, *lexema))){
            linhaAtual = atoi(lexema);
            printf("%d ", linhaAtual);
            insereLINE( tabela, linhaAtual, contadorInstrucaoPtr, contadorTabelaPtr );
        }
        else{
            printf("\n\tERRO LINHA: '%2s' Invalida OU ja existente", lexema);
            getchar();
            exit(0);
        }

        //segundo lexema deve ser um comando valido(let, rem, ...)
        lexema = strtok(NULL, " ");
        strcpy(instrucao, lexema); // copia da instrução, caso precise, pra não perder
        switch (tipoComando(instrucao)){
            case rem:
                //Não fazer nada;
                break;

            case input:
                //se input, o prox lexema deve ser uma variavel válida
                lexema = strtok(NULL, " ");
                if (isVar(lexema)){
                    //busca, se não encontrar, aloca
                    if (!busca(tabela, *lexema))
                        insereVAR( tabela, *lexema, contadorDadosPtr, contadorTabelaPtr);
                    LM[contadorInstrucao++] = READ + tabela[ busca( tabela, *lexema) ].location;
                }
                else{
                    //variavel invalida
                    printf("\nErro Lexico, variavel invalida na linha %d", linhaAtual);
                }
                break;

            case print:
                lexema = strtok( NULL, " " );
                //se print, o prox lexema deve ser uma variavel válida
                if (isVar(lexema)){
                    //busca, se não encontrar, aloca
                    if (!busca(tabela, *lexema))
                        insereVAR( tabela, *lexema, contadorDadosPtr, contadorTabelaPtr);
                    LM[contadorInstrucao++] = WRITE + tabela[ busca( tabela, *lexema) ].location;
                }
                else{
                    //variavel invalida
                    printf("\nErro Lexico, variavel invalida na linha %d", linhaAtual);
                }
                break;

            case let:
                lexema = strtok( NULL, " " );
                // se let, o prox lexema deve ser variavel
                if (isVar(lexema)){
                    if( !busca(tabela, *lexema) )
                        insereVAR( tabela, *lexema, contadorDadosPtr, contadorTabelaPtr );
                    store =  tabela[busca(tabela, *lexema) ].location;

                    lexema = strtok( NULL, " " );
                    // Logo apos, deve ahver o sinal de atribuição
                    if (lexema == "="){
                        lexema = strtok( NULL, " " );
                        infix[0] = '\0'; // reset string
                        while ( lexema != NULL ) {
                            if( isVar( *lexema ) )
                                if( !busca(tabela, *lexema) )
                                    insereVAR( tabela, *lexema, contadorDadosPtr, contadorTabelaPtr );
                            if( isNum( *lexema ) )
                                if( !busca(tabela, atoi(lexema)) ) {
                                    InsereCONST( tabela, atoi( lexema ), contadorDadosPtr, contadorTabelaPtr );
                                    LM[ contadorInstrucao++ ] = 1300 + tabela[busca(tabela, atoi(lexema)) ].location;
                                    LM[ contadorInstrucao++ ] = atoi( lexema );
                                }
                            strcat( infix, lexema );
                            strcat( infix, " ");
                            lexema = strtok( NULL, " " );
                        }

                        // Se a expressão é valida, convertemos
                        if (isValidExp(infix)){
                            postfix[0] = '\0';
                            In2Pos(infix, postfix);
                        }
                        else{
                            //expressão mal formatada
                            exit(-1);
                        }
                        // montando as instruçoes usando sintaxe de posFixagem
                        lexema = strtok( postfix, " " );
                        while( lexema != NULL ) {
                            if( isNum( lexema ) )
                                push( stack, tabela[busca(tabela, atoi(lexema)) ].location );
                            else if( isVar( lexema ) )
                                push( stack, tabela[busca(tabela, *lexema) ].location );
                            else if( isOp( lexema ) ) {
                                int locAux = + pop(stack);
                                LM[ contadorInstrucao++ ] = 2000 + pop( stack );
                                LM[ contadorInstrucao++ ] = operador(*lexema) + locAux;
                                LM[ contadorInstrucao++ ] = 2100 + contadorDados;
                                push( stack, contadorDados-- );
                            }
                            lexema = strtok( NULL, " " );
                        }
                        LM[ contadorInstrucao++ ] = LOAD + pop(stack);
                        LM[ contadorInstrucao++ ] = STORE + store;
                    }
                    else{
                        //erro, sintaxe "let var = exp"
                        exit(-1);
                    }
                }
                else{
                    //erro, sintaxe "let var = exp"
                    exit(-1);
                }
                break;

            case goto_:
                lexema = strtok( NULL, " " );
                if (isNum(lexema)){
                    if( !busca(tabela, atoi(lexema)) ) {
                        flags[ contadorInstrucao ] = atoi( lexema );
                        LM[ contadorInstrucao++ ] = 4000; // quando a linha é (ou por enquanto é) inexistente
                    }
                    else
                        LM[ contadorInstrucao++ ] = 4000 + tabela[busca(tabela, atoi(lexema)) ].location;
                }
                else{
                    printf("\n\tERRO LINHA: '%2s' Invalida", lexema);
                    getchar();
                    exit(0);
                }
            case if_:
            case end:
                LM[ contadorInstrucao++ ] = 4300;
            default:
                printf("\n\tERRO COMANDO '%s' INVALIDO", lexema);
                getchar();
                exit(0);
        }


    }

    getchar();
}