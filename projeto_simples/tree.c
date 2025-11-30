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

// --- PARTE 1: MANIPULAÇÃO DA ÁRVORE ---

/* Aloca memória e cria um novo nó genérico (para números ou operadores). */
ptno criaNo(int tipo, int valor)
{
    ptno n = (ptno)malloc(sizeof(struct no));
    n->tipo = tipo;
    n->valor = valor;
    n->nome[0] = '\0';
    n->nfilhos = 0;
    for(int i=0; i<MAXFILHOS; i++) n->filho[i] = NULL;
    return n;
}

/* Aloca memória e cria um novo nó específico para guardar texto (identificadores). */
ptno criaNoStr(int tipo, const char *str)
{
    ptno n = criaNo(tipo, 0);
    strcpy(n->nome, str);
    return n;
}

/* Conecta um nó 'filho' ao vetor de filhos do nó 'pai', montando a hierarquia. */
void adicionaFilho(ptno pai, ptno filho)
{
    if (filho == NULL) return;
    if (pai->nfilhos < MAXFILHOS)
        pai->filho[pai->nfilhos++] = filho;
}

// --- PARTE 2: GERAÇÃO DO GRÁFICO (.DOT) ---

FILE *gdot;

/* Função recursiva auxiliar que escreve os dados de cada nó e desenha as setas no arquivo .dot. */
void escreveNo(ptno n)
{
    if (!n) return;

    fprintf(gdot, "  nodo%p [label=\"", (void*)n);

    switch (n->tipo) {
        case N_PROGRAM: fprintf(gdot, "PROGRAMA"); break;
        case N_LISTA:   fprintf(gdot, "LISTA_CMDS"); break;
        case N_DECL_VAR:fprintf(gdot, "DECLARACOES"); break;
        case N_ATR:     fprintf(gdot, "="); break;
        case N_LEIA:    fprintf(gdot, "LEIA"); break;
        case N_ESCREVA: fprintf(gdot, "ESCREVA"); break;
        case N_SE:      fprintf(gdot, "SE"); break;
        case N_ENQUANTO:fprintf(gdot, "ENQUANTO"); break;
        case N_ID:      fprintf(gdot, "ID: %s", n->nome); break;
        case N_NUM:     fprintf(gdot, "NUM: %d", n->valor); break;
        case N_PLUS:    fprintf(gdot, "+"); break;
        case N_MINUS:   fprintf(gdot, "-"); break;
        case N_MUL:     fprintf(gdot, "*"); break;
        case N_DIV:     fprintf(gdot, "/"); break;
        case N_MAIOR:   fprintf(gdot, ">"); break;
        case N_MENOR:   fprintf(gdot, "<"); break;
        case N_IGUAL:   fprintf(gdot, "=="); break;
        case N_DIF:     fprintf(gdot, "!="); break;
        case N_MAIG:    fprintf(gdot, ">="); break;
        case N_MENIG:   fprintf(gdot, "<="); break;
    }

    fprintf(gdot, "\"];\n");

    for (int i = 0; i < n->nfilhos; i++) {
        if (n->filho[i] != NULL) {
            fprintf(gdot, "  nodo%p -> nodo%p;\n", (void*)n, (void*)n->filho[i]);
            escreveNo(n->filho[i]);
        }
    }
}

/* Função principal de desenho: abre o arquivo e dispara a escrita recursiva da árvore. */
void geraDot(ptno raiz, const char *arquivo)
{
    gdot = fopen(arquivo, "w");
    if (!gdot) {
        fprintf(stderr, "Erro ao criar arquivo .dot\n");
        return;
    }
    fprintf(gdot, "digraph G {\n");
    escreveNo(raiz);
    fprintf(gdot, "}\n");
    fclose(gdot);
}

// --- PARTE 3: GERAÇÃO DE CÓDIGO MVS ---

char *tabela[100];
int pos = 0;

/* Adiciona uma nova variável na tabela de símbolos e retorna seu endereço de memória. */
int registraID(const char *id) {
    for (int i = 0; i < pos; i++)
        if (strcmp(tabela[i], id) == 0)
            return i;
    tabela[pos] = strdup(id);
    return pos++;
}

/* Busca o endereço de uma variável na tabela (se não existir, cria). */
int buscaID(const char *id) {
    return registraID(id);
}

/* Percorre a árvore antes da geração de código para encontrar e registrar todas as variáveis usadas. */
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

/* Função principal da geração: emite o cabeçalho (INPP), aloca memória (AMEM) e chama a geração dos comandos. */
void geracod(ptno raiz, FILE *f) {
    fprintf(f, "INPP\n");
    
    preProcessaVariaveis(raiz->filho[1]); 
    preProcessaVariaveis(raiz->filho[2]); 
    
    if (pos > 0) {
        fprintf(f, "AMEM %d\n", pos);
    }

    geraComandos(raiz->filho[2], f); 
    
    fprintf(f, "PARA\n");
}

/* Percorre a árvore de comandos e traduz cada estrutura (SE, ENQUANTO, ATRIBUIÇÃO) para instruções MVS. */
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

/* Traduz expressões matemáticas e lógicas para instruções de pilha da MVS. */
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