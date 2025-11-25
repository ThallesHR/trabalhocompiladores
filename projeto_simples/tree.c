/*+=============================================================
| UNIFAL = Universidade Federal de Alfenas.
| BACHARELADO EM CIENCIA DA COMPUTACAO.
| Trabalho . . : Construcao Arvore Sintatica e Geracao de Codigo
| Disciplina : Teoria de Linguagens e Compiladores
| Professor . : Luiz Eduardo da Silva
| Aluno . . . . . : Thalles Henrique Gonzaga Rosa Pereira
| Data . . . . . . : 20/11/2025
+=============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"


ptno criaNo(int tipo, int valor)
{
    ptno n = (ptno)malloc(sizeof(struct no));
    n->tipo = tipo;
    n->valor = valor;
    n->nome[0] = '\0';
    n->nfilhos = 0;
    return n;
}


ptno criaNoStr(int tipo, const char *str)
{
    ptno n = criaNo(tipo, 0);
    strcpy(n->nome, str);
    return n;
}


void adicionaFilho(ptno pai, ptno filho)
{
    if (pai->nfilhos < MAXFILHOS)
        pai->filho[pai->nfilhos++] = filho;
}



FILE *gdot;

void escreveNo(ptno n)
{
    if (!n) return;

    fprintf(gdot, "  nodo%p [label=\"", (void*)n);

    switch (n->tipo) {
        case N_PROGRAM: fprintf(gdot, "PROGRAM"); break;
        case N_LISTA: fprintf(gdot, "LISTA"); break;
        case N_ATR: fprintf(gdot, "ATR"); break;
        case N_LEIA: fprintf(gdot, "LEIA"); break;
        case N_ESCREVA: fprintf(gdot, "ESCREVA"); break;
        case N_ID: fprintf(gdot, "ID: %s", n->nome); break;
        case N_NUM: fprintf(gdot, "NUM: %d", n->valor); break;
        case N_PLUS: fprintf(gdot, "+"); break;
        case N_MINUS: fprintf(gdot, "-"); break;
        case N_MUL: fprintf(gdot, "*"); break;
        case N_DIV: fprintf(gdot, "/"); break;
    }

    fprintf(gdot, "\"];\n");

    for (int i = 0; i < n->nfilhos; i++) {
        fprintf(gdot, "  nodo%p -> nodo%p;\n", (void*)n, (void*)n->filho[i]);
        escreveNo(n->filho[i]);
    }
}

void geraDot(ptno raiz, const char *arquivo)
{
    gdot = fopen(arquivo, "w");
    fprintf(gdot, "digraph G {\n");
    escreveNo(raiz);
    fprintf(gdot, "}\n");
    fclose(gdot);
}
