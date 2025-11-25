/*+=============================================================
| UNIFAL = Universidade Federal de Alfenas.
| BACHARELADO EM CIENCIA DA COMPUTACAO.
| Trabalho . . : Construcao Arvore Sintatica e Geracao de Codigo
| Disciplina : Teoria de Linguagens e Compiladores
| Professor . : Luiz Eduardo da Silva
| Aluno . . . . . : Fulano da Silva
| Data . . . . . . : 99/99/9999
+=============================================================*/

#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#define MAXFILHOS 10

/* tipos de nó da AST */
enum {
    N_PROGRAM,
    N_LISTA,
    N_ATR,
    N_LEIA,
    N_ESCREVA,
    N_ID,
    N_NUM,
    N_PLUS,
    N_MINUS,
    N_MUL,
    N_DIV
};

/* nó da AST */
typedef struct no {
    int tipo;
    int valor;      
    char nome[50]; 
    struct no *filho[MAXFILHOS];
    int nfilhos;
} *ptno;

/* funções */
ptno criaNo(int tipo, int valor);
ptno criaNoStr(int tipo, const char *str);
void adicionaFilho(ptno pai, ptno filho);

void geraDot(ptno raiz, const char *arquivo);

#endif
