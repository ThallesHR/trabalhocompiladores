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
#include "tree.h"
#include <string.h>

char *tabela[100];
int pos = 0;

int registraID(const char *id) {
    for (int i = 0; i < pos; i++)
        if (strcmp(tabela[i], id) == 0)
            return i;
    tabela[pos] = strdup(id);
    return pos++;
}

int buscaID(const char *id) {
    return registraID(id);
}

// Varre a árvore procurando IDs para registrar antes de gerar código
void preProcessaVariaveis(ptno no) {
    if (no == NULL) return;
    if (no->tipo == N_ID) {
        registraID(no->nome);
    }
    for (int i = 0; i < no->nfilhos; i++) {
        preProcessaVariaveis(no->filho[i]);
    }
}

void geraExpressao(ptno no, FILE *f);
void geraComandos(ptno no, FILE *f);

void geracod(ptno raiz, FILE *f) {
    fprintf(f, "INPP\n");
    
    // Varre declarações E comandos para achar todas as variáveis usadas
    preProcessaVariaveis(raiz->filho[1]); // Declarações explícitas
    preProcessaVariaveis(raiz->filho[2]); // Variáveis usadas nos comandos
    
    // Reserva memória para todas as variáveis encontradas
    if (pos > 0) {
        fprintf(f, "AMEM %d\n", pos);
    }

    geraComandos(raiz->filho[2], f); // Gera o código
    
    fprintf(f, "PARA\n");
}

void geraComandos(ptno no, FILE *f) {
    if (no == NULL) return;

    for (int i = 0; i < no->nfilhos; i++) {
        ptno cmd = no->filho[i];
        if (!cmd) continue;

        switch (cmd->tipo) {
        case N_ATR: {
            int posVar = buscaID(cmd->filho[0]->nome);
            geraExpressao(cmd->filho[1], f);
            fprintf(f, "ARMZ %d\n", posVar);
            break;
        }
        case N_LEIA: {
            int posVar = buscaID(cmd->filho[0]->nome);
            fprintf(f, "LEIT\n");
            fprintf(f, "ARMZ %d\n", posVar);
            break;
        }
        case N_ESCREVA: {
            geraExpressao(cmd->filho[0], f);
            fprintf(f, "IMPR\n");
            break;
        }
        case N_SE: {
            int rotuloSenao = pos + 1000 + rand() % 1000;
            int rotuloFim = pos + 2000 + rand() % 1000;
            
            geraExpressao(cmd->filho[0], f);
            fprintf(f, "DSVF L%d\n", rotuloSenao);
            
            geraComandos(cmd->filho[1], f);
            fprintf(f, "DSVS L%d\n", rotuloFim);
            
            fprintf(f, "L%d: NADA\n", rotuloSenao);
            if (cmd->nfilhos > 2) {
                geraComandos(cmd->filho[2], f);
            }
            fprintf(f, "L%d: NADA\n", rotuloFim);
            break;
        }
        case N_ENQUANTO: {
            int rotuloInicio = pos + 3000 + rand() % 1000;
            int rotuloFim = pos + 4000 + rand() % 1000;

            fprintf(f, "L%d: NADA\n", rotuloInicio);
            geraExpressao(cmd->filho[0], f);
            fprintf(f, "DSVF L%d\n", rotuloFim);

            geraComandos(cmd->filho[1], f);
            fprintf(f, "DSVS L%d\n", rotuloInicio);

            fprintf(f, "L%d: NADA\n", rotuloFim);
            break;
        }
        }
    }
}

void geraExpressao(ptno no, FILE *f) {
    if (no == NULL) return;

    switch (no->tipo) {
    case N_NUM: fprintf(f, "CRCT %d\n", no->valor); break;
    case N_ID:  fprintf(f, "CRVL %d\n", buscaID(no->nome)); break;
    case N_PLUS:  geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "SOMA\n"); break;
    case N_MINUS: geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "SUBT\n"); break;
    case N_MUL:   geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "MULT\n"); break;
    case N_DIV:   geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "DIVI\n"); break;
    
    case N_MAIOR: geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "CMMA\n"); break;
    case N_MENOR: geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "CMME\n"); break;
    case N_IGUAL: geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "CMIG\n"); break;
    case N_DIF:   geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "CMDG\n"); break;
    case N_MAIG:  geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "CMAG\n"); break;
    case N_MENIG: geraExpressao(no->filho[0], f); geraExpressao(no->filho[1], f); fprintf(f, "CMEG\n"); break;
    }
}