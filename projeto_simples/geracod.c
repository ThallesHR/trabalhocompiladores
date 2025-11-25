#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include <string.h>
#include <stdlib.h>
int buscaID(const char *id);


char *tabela[100];
int pos = 0;

int registraID(const char *id)
{
    for (int i = 0; i < pos; i++)
        if (strcmp(tabela[i], id) == 0)
            return i;

    tabela[pos] = strdup(id);
    return pos++;
}

int buscaID(const char *id)
{
    for (int i = 0; i < pos; i++)
        if (strcmp(tabela[i], id) == 0)
            return i;

    return registraID(id);
}



void geraExpressao(ptno no, FILE *f);
void geraComandos(ptno no, FILE *f);

void geracod(ptno raiz, FILE *f)
{
    fprintf(f, "INPP\n");  

    geraComandos(raiz->filho[1], f);

    fprintf(f, "PARA\n"); 
}

void geraComandos(ptno no, FILE *f)
{
    for (int i = 0; i < no->nfilhos; i++) {
        ptno cmd = no->filho[i];

        switch (cmd->tipo) {

        case N_ATR: {
            int posVar = buscaID(cmd->filho[0]->nome);
            geraExpressao(cmd->filho[1], f);
            fprintf(f, "ARMZ %d\n", posVar);
            break;
        }

        case N_LEIA: {
            int posVar = buscaID(cmd->filho[0]->nome);
            fprintf(f, "LEIT\nARMZ %d\n", posVar);
            break;
        }

        case N_ESCREVA: {
            geraExpressao(cmd->filho[0], f);
            fprintf(f, "IMPR\n");
            break;
        }
        }
    }
}


void geraExpressao(ptno no, FILE *f)
{
    switch (no->tipo) {

    case N_NUM:
        fprintf(f, "CRCT %d\n", no->valor);
        break;

    case N_ID: {
        int posVar = buscaID(no->nome);
        fprintf(f, "CRVL %d\n", posVar);
        break;
    }

    case N_PLUS:
        geraExpressao(no->filho[0], f);
        geraExpressao(no->filho[1], f);
        fprintf(f, "SOMA\n");
        break;

    case N_MINUS:
        geraExpressao(no->filho[0], f);
        geraExpressao(no->filho[1], f);
        fprintf(f, "SUBT\n");
        break;

    case N_MUL:
        geraExpressao(no->filho[0], f);
        geraExpressao(no->filho[1], f);
        fprintf(f, "MULT\n");
        break;

    case N_DIV:
        geraExpressao(no->filho[0], f);
        geraExpressao(no->filho[1], f);
        fprintf(f, "DIVI\n");
        break;
    }
}
