/*+=============================================================
| UNIFAL = Universidade Federal de Alfenas.
| BACHARELADO EM CIENCIA DA COMPUTACAO.
| Trabalho . . : Construcao Arvore Sintatica e Geracao de Codigo
| Disciplina : Teoria de Linguagens e Compiladores
| Professor . : Luiz Eduardo da Silva
| Aluno . . . . . : Thalles Henrique Gonzaga Rosa Pereira
| Data . . . . . . : 20/11/2025
+=============================================================*/

#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#define MAXFILHOS 10

/* ENUM: Lista de códigos (etiquetas) para identificar o tipo de cada nó da árvore. */
enum {
    N_PROGRAM,
    N_LISTA,
    N_DECL_VAR,
    N_ATR,
    N_LEIA,
    N_ESCREVA,
    N_SE,
    N_ENQUANTO,
    N_PLUS,
    N_MINUS,
    N_MUL,
    N_DIV,
    N_MAIOR,
    N_MENOR,
    N_IGUAL,
    N_DIF,
    N_MAIG,
    N_MENIG,
    N_ID,
    N_NUM
};

/* STRUCT: A estrutura de dados que representa um único nó da árvore na memória. */
typedef struct no {
    int tipo;
    int valor;
    char nome[50];
    struct no *filho[MAXFILHOS];
    int nfilhos;
} *ptno;

/* Protótipos das funções implementadas em tree.c */
ptno criaNo(int tipo, int valor);
ptno criaNoStr(int tipo, const char *str);
void adicionaFilho(ptno pai, ptno filho);
void geraDot(ptno raiz, const char *arquivo);
void geracod(ptno raiz, FILE *f);

#endif