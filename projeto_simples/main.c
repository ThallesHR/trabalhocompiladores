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

extern void erro(const char *msg); 
extern FILE *yyin; 
extern ptno raiz;
extern int yyparse();

void geraDot(ptno raiz, const char *nomeArquivo);
void geracod(ptno raiz, FILE *f);

// Pega apenas o nome do arquivo (sem extensão) para nomear as saídas (.dot, .mvs)
void get_base_name(const char *full_path, char *base_name) {
    const char *last_slash = strrchr(full_path, '/');
    if (!last_slash) last_slash = strrchr(full_path, '\\');
    const char *filename = last_slash ? last_slash + 1 : full_path;
    char *dot = strrchr(filename, '.');
    size_t length = dot ? (size_t)(dot - filename) : strlen(filename);
    strncpy(base_name, filename, length);
    base_name[length] = '\0';
}

// Função principal: Abre o arquivo, inicia a análise (yyparse) e chama as funções de geração de arquivos
int main(int argc, char **argv)
{
    char nome_base[256];
    char cmd_dot[640]; 
    char saida_dot[300], saida_mvs[300], saida_svg[300]; 

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_fonte.simples>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        erro("Nao foi possivel abrir o arquivo.");
        return 1;
    }

    get_base_name(argv[1], nome_base);
    snprintf(saida_dot, sizeof(saida_dot), "%s.dot", nome_base);
    snprintf(saida_svg, sizeof(saida_svg), "%s.svg", nome_base);
    snprintf(saida_mvs, sizeof(saida_mvs), "%s.mvs", nome_base);

    if (yyparse() == 0) {
        printf("Analise sintatica concluida.\n");

        geraDot(raiz, saida_dot);
        
        sprintf(cmd_dot, "dot -Tsvg %s -o %s", saida_dot, saida_svg);
        system(cmd_dot);
        
        FILE *f = fopen(saida_mvs, "w");
        geracod(raiz, f);
        fclose(f);

        printf("Codigo MVS gerado.\n");
        printf("Codigo DOT gerado.\n");
        printf("Arquivo SVG gerado.\n");
    } else {
        printf("Erros durante a analise.\n");
    }

    return 0;
}