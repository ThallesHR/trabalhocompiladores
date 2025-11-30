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

// Lista de códigos para identificar o tipo de cada nó da árvore
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

// guarda o tipo do nó, seu valor/nome e os ponteiros para os filhos
typedef struct no {
    int tipo;
    int valor;
    char nome[50];
    struct no *filho[MAXFILHOS];
    int nfilhos;
} *ptno;

// Cria um nó simples 
ptno criaNo(int tipo, int valor);

// Cria um nó que guarda texto 
ptno criaNoStr(int tipo, const char *str);

// Adiciona um nó filho a um nó pai 
void adicionaFilho(ptno pai, ptno filho);

// Gera o arquivo de texto (.dot) que será usado para desenhar o gráfico da árvore
void geraDot(ptno raiz, const char *arquivo);

#endif