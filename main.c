#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#include "TableEntry.h"
#include "comandos.h"
#include "expConvertion.h"
#include "PilhaEnderecos.h"

int main() {
    TableEntry tabela[MAX];
    int flags[100];         //usado para resolução de endereços faltantes de linhas
    int LM[100] = { 0 };    // Linguagem de Maquina, instruçoes;
    int contadorDados = 99;
    int contInstrucao = 0;
    int contadorTabela = 1;
    int *contadorInstrucaoPtr = &contInstrucao;
    int *contadorDadosPtr = &contadorDados;
    int *contadorTabelaPtr = &contadorTabela;

    char linha[1000];
    char *lexema;
    char instrucao[10];
    int linhaAtual;
    int i;

    for( i = 0; i < 100; ++i ) flags[i] = -1; // iniciar flags de endereço de linha

    // Para uso do 'let'
    char infix[50] = { 0 };
    char postfix[50] = { 0 };
    int store;
    Stack stack = criaStack();

    //if...goto
    int OPLOGICO;

    char nome[100];
    FILE *codigoFonte = NULL, *smlOUT = NULL;

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
        if (isNum(lexema) && (atoi(lexema) >= 0 && atoi(lexema) <= 99) && !(busca(tabela, *lexema, LINE))){
            linhaAtual = atoi(lexema);
            printf("%d ", linhaAtual);
            insereLINE( tabela, linhaAtual, contadorInstrucaoPtr, contadorTabelaPtr );
        }
        else{
            printf("\n\tERRO LINHA: '%2s' Invalida OU ja existente", lexema);
            getch();
            exit(0);
        }

        //segundo lexema deve ser um comando valido(let, rem, ...)
        lexema = strtok(NULL, " ");
        strcpy(instrucao, lexema); // copia da instrução, caso precise, pra não perder
        switch (tipoComando(instrucao)) {
            case rem:
                //  Não fazer nada;
                break;
                //  FIM REM
            case input:
                //se input, o prox lexema deve ser uma variavel válida
                lexema = strtok(NULL, " ");
                if (isVar(lexema)) {
                    //busca, se não encontrar, aloca
                    if (!busca(tabela, *lexema, VAR))
                        insereVAR(tabela, *lexema, contadorDadosPtr, contadorTabelaPtr);
                    LM[contInstrucao++] = READ + tabela[busca(tabela, *lexema, VAR)].location;
                }
                else {
                    //variavel invalida
                    printf("\nErro Lexico, variavel invalida na linha %d", linhaAtual);
                    getch();
                }
                break;
                //  FIM INPUT
            case print:
                lexema = strtok(NULL, " ");
                //se print, o prox lexema deve ser uma variavel válida
                if (isVar(lexema)) {
                    //busca, se não encontrar, aloca
                    if (!busca(tabela, *lexema, VAR))
                        insereVAR(tabela, *lexema, contadorDadosPtr, contadorTabelaPtr);
                    LM[contInstrucao++] = WRITE + tabela[busca(tabela, *lexema, VAR)].location;
                }
                else {
                    //variavel invalida
                    printf("\nErro Lexico, variavel invalida na linha %d", linhaAtual);
                    getch();
                }
                break;
                //  FIM PRINT
            case let:
                lexema = strtok(NULL, " ");
                // se let, o prox lexema deve ser variavel
                if (isVar(lexema)) {
                    if (!busca(tabela, *lexema, VAR))
                        insereVAR(tabela, *lexema, contadorDadosPtr, contadorTabelaPtr);
                    store = tabela[busca(tabela, *lexema, VAR)].location;

                    lexema = strtok(NULL, " ");
                    // Logo apos, deve ahver o sinal de atribuição
                    if (!strcmp(lexema, "=")) {
                        lexema = strtok(NULL, " ");
                        infix[0] = '\0'; // reset string
                        while (lexema != NULL) {
                            if (isVar(lexema))
                                if (!busca(tabela, *lexema, VAR))
                                    insereVAR(tabela, *lexema, contadorDadosPtr, contadorTabelaPtr);
                            if (isNum(lexema))
                                if (!busca(tabela, atoi(lexema), CONSTC)) {
                                    InsereCONST(tabela, atoi(lexema), contadorDadosPtr, contadorTabelaPtr);
                                    //prepara para atribuição de valor a memória
                                    LM[contInstrucao++] = WCONST + tabela[busca(tabela, atoi(lexema), CONSTC)].location;
                                    LM[contInstrucao++] = atoi(lexema);
                                }
                            strcat(infix, lexema);
                            strcat(infix, " ");
                            lexema = strtok(NULL, " ");
                        }

                        // Se a expressão é valida, convertemos
                        if (isValidExp(infix)) {
                            postfix[0] = '\0';
                            In2Pos(infix, postfix);
                        }
                        else {
                            //expressão mal formatada
                            getch();
                            exit(-1);
                        }
                        // montando as instruçoes usando sintaxe de posFixagem
                        lexema = strtok(postfix, " ");
                        while (lexema != NULL) {
                            if (isNum(lexema))
                                push(stack, tabela[busca(tabela, atoi(lexema), CONSTC)].location);
                            else if (isVar(lexema))
                                push(stack, tabela[busca(tabela, *lexema, VAR)].location);
                            else if (isOp(lexema)) {
                                int locAux = +pop(stack);                              //remove e Guarda o topo
                                LM[contInstrucao++] = 2000 + pop(stack);        //remove e carrega o novo topo
                                LM[contInstrucao++] = operador(*lexema) +
                                                      locAux; //opera sobre o acumulador, usando o topo guardado anteriormente
                                LM[contInstrucao++] = 2100 + contadorDados;       //armazena numa posição auxiliar
                                push(stack, contadorDados--);                         //empilha o ultimo resultado
                            }
                            lexema = strtok(NULL, " ");
                        }
                        LM[contInstrucao++] = LOAD + pop(stack);      //carregar resultado final
                        LM[contInstrucao++] = STORE + store;          //salvar na variavel inicial, guardado em 'store'
                    }
                    else {
                        //erro, sintaxe "let var = exp"
                        getch();
                        exit(-1);
                    }
                }
                else {
                    //erro, sintaxe "let var = exp"
                    getch();
                    exit(-1);
                }
                break;
                //  Fim LET
            case goto_:
                lexema = strtok(NULL, " ");
                if (isNum(lexema)) {
                    if (!busca(tabela, atoi(lexema), LINE)) {
                        flags[contInstrucao] = atoi(lexema);
                        LM[contInstrucao++] = 4000; // quando a linha é (ou por enquanto é) inexistente
                    }
                    else
                        LM[contInstrucao++] = 4000 + tabela[busca(tabela, atoi(lexema), LINE)].location;
                }
                else {
                    printf("\n\tERRO LINHA: '%2s' Invalida", lexema);
                    getch();
                    exit(0);
                }
                break;
                //  FIM GOTO
            case if_:
                // Primeira Variavel ou constante
                // LOAD
                lexema = strtok(NULL, " ");
                if (isVar(lexema)) {
                    if (!busca(tabela, *lexema, VAR))
                        insereVAR(tabela, *lexema, contadorDadosPtr, contadorTabelaPtr);
                    LM[contInstrucao++] =
                            LOAD + tabela[busca(tabela, *lexema, VAR)].location;    //carrega o valor no acumulador
                }
                else if (isNum(lexema)) {
                    if (!busca(tabela, atoi(lexema), CONSTC)) {
                        InsereCONST(tabela, atoi(lexema), contadorDadosPtr, contadorTabelaPtr);
                        //prepara para atribuição de valor a memória
                        LM[contInstrucao++] = WCONST + tabela[busca(tabela, atoi(lexema), CONSTC)].location;
                        LM[contInstrucao++] = atoi(lexema);
                    }
                    LM[contInstrucao++] = LOAD + tabela[busca(tabela, atoi(lexema),
                                                              CONSTC)].location;  //carrega o valor no acumulador
                }
                else {
                    //depois do if deve ser ou Variavel, ou Constante
                    //invalido
                    getch();
                    exit(-1);
                }

                // Obtendo operador logico
                lexema = strtok(NULL, " ");
                if ((OPLOGICO = classifLogicos(lexema)) == -1) {
                    getch();
                    exit(-1); // Operador invalido
                }

                // Proxima Variavel ou Constante
                //SUB
                lexema = strtok(NULL, " ");
                if (isVar(lexema)) {
                    if (!busca(tabela, *lexema, VAR))
                        insereVAR(tabela, *lexema, contadorDadosPtr, contadorTabelaPtr);
                    LM[contInstrucao++] =
                            SUB + tabela[busca(tabela, *lexema, VAR)].location;    //Subtrai do valor no acumulador
                }
                else if (isNum(lexema)) {
                    if (!busca(tabela, atoi(lexema), CONSTC)) {
                        //prepara para atribuição de valor a memória
                        InsereCONST(tabela, atoi(lexema), contadorDadosPtr, contadorTabelaPtr);
                        LM[contInstrucao++] = WCONST + tabela[busca(tabela, atoi(lexema), CONSTC)].location;
                        LM[contInstrucao++] = atoi(lexema);
                    }
                    LM[contInstrucao++] = SUB + tabela[busca(tabela, atoi(lexema),
                                                             CONSTC)].location;  //Subtrai do valor no acumulador
                }
                else {
                    //depois do if deve ser ou Variavel, ou Constante
                    //invalido
                    getch();
                    exit(-1);
                }

                lexema = strtok(NULL, " ");
                if (strcmp(lexema, "goto")) {
                    getch();
                    exit(-1); // Depois da expressão logica, deve-se haver um 'goto'
                }

                lexema = strtok( NULL, " " );
                if (!isNum(lexema) && (atoi(lexema) < 0 || atoi(lexema) > 99)){
                    exit(-1);   // Depois do 'goto' deve haver uma linha para desviar
                                // e essa deve ser um numero válido no intervalo (0,99)
                }

                switch (OPLOGICO){
                    case IGUAL:
                        flags[ contInstrucao ] = atoi( lexema );
                        LM[ contInstrucao++ ] = BRANCHZERO;         //Se a a-b=0, então a==b, desvia os fluxo
                        break;
                    case DIFERENTE:
                        LM[ contInstrucao ] = BRANCHZERO + contInstrucao + 2;       //se acumulador=0(errado) (a==b)
                        contInstrucao++;                                            // pula a instrução Auxiliar
                        flags[ contInstrucao ] = atoi( lexema );                    // se acumulador!=0(certo) (a!=b), segue o fluxo
                        LM[ contInstrucao++ ] = BRANCH;                             // instrução auxilixar que redireciona o fluxo, é executada
                        break;
                    case MENOR:
                        flags[ contInstrucao ] = atoi( lexema );
                        LM[ contInstrucao++ ] = BRANCHNEG;          //Se a-b<0, então a<b, desvia o fluxo
                        break;
                    case MENORGUAL:
                        flags[ contInstrucao ] = atoi( lexema );    //Verifica se é menor
                        LM[ contInstrucao++ ] = BRANCHNEG;
                        flags[ contInstrucao ] = atoi( lexema );    //verifica se é igual
                        LM[ contInstrucao++ ] = BRANCHZERO;         //Se passar por qualquer um dos dois, desvia o fluxo
                    case MAIOR:
                        LM[ contInstrucao ] = BRANCHNEG + contInstrucao + 2;    //Verifica se é maior
                        contInstrucao++;
                        flags[ contInstrucao ] = atoi( lexema );
                        LM[ contInstrucao++ ] = BRANCH;                         //Se maior, executa essa instrução, para desviar
                        break;                                                  //Se não, essa instrução é pulada
                    case MAIORIGUAL:
                        LM[ contInstrucao ] = BRANCHNEG + contInstrucao + 2;    //Verifica se é maior
                        contInstrucao++;
                        flags[ contInstrucao ] = atoi( lexema );
                        LM[ contInstrucao++ ] = BRANCH;
                        flags[ contInstrucao ] = atoi( lexema );                //verifica se é igual
                        LM[ contInstrucao++ ] = BRANCHZERO;                     //Se passar por qualquer um dos dois, desvia o fluxo
                        break;
                }

            case end:
                LM[ contInstrucao++ ] = 4300;
                break;
            default:
                printf("\n\tERRO COMANDO '%s' INVALIDO", lexema);
                getchar();
                exit(0);
        }
    }
    fclose(codigoFonte); //fechando arquivo fonte

    for( i = 0; i < 100; ++i ) {
        if (flags[i] != -1) {
            LM[i] += tabela[busca(tabela, flags[i], LINE)].location;   //  Passa procurando os endereços não
        }                                                              //resolvidos anteriormente nos "goto"
    }

    while (smlOUT == NULL){
        system("cls");
        printf("\n\n\tDigite o nome do Codigo-fonte: ");
        gets(nome);
        system("cls");
        if ((smlOUT = fopen(nome, "w")) == NULL){
            printf("\n\n\tErro ao abrir arquivo.");
        }
        else{
            printf("\n\n\tSucesso! Salvando!");
        }
        Sleep(1000);
        system("cls");
    }

    //Escrevendo no arquivo;
    for( i = 0; LM[i] != 0; ++i )
        fprintf( smlOUT, "%d\n", LM[i] );

    fflush(smlOUT);
    fclose(smlOUT);
}