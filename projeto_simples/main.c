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


void get_base_name(const char *full_path, char *base_name) {
    const char *last_slash = strrchr(full_path, '/');
    if (!last_slash) {
        last_slash = strrchr(full_path, '\\');
    }
    const char *filename = last_slash ? last_slash + 1 : full_path;
    
    char *dot = strrchr(filename, '.');
    size_t length = dot ? (size_t)(dot - filename) : strlen(filename);
    
    strncpy(base_name, filename, length);
    base_name[length] = '\0';
}

int main(int argc, char **argv)
{
    char nome_base[256];
    char cmd_dot[512];
    char saida_dot[256];
    char saida_mvs[256];
    char saida_svg[256];

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_fonte.simples>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        erro("Nao foi possivel abrir o arquivo de entrada.");
        return 1;
    }

    get_base_name(argv[1], nome_base);
    
    sprintf(saida_dot, "%s.dot", nome_base);
    sprintf(saida_svg, "%s.svg", nome_base);
    sprintf(saida_mvs, "%s.mvs", nome_base);

    if (yyparse() == 0) {
        printf("Analise sintatica concluida.\n");

        geraDot(raiz, saida_dot);
        printf("Arquivo AST gerado: %s\n", saida_dot);

        sprintf(cmd_dot, "dot -Tsvg %s -o %s", saida_dot, saida_svg);
        if (system(cmd_dot) == 0) {
            printf("Arquivo SVG gerado: %s\n", saida_svg);
        } else {
            fprintf(stderr, "Aviso: Nao foi possivel gerar %s. Graphviz ('dot') pode nao estar instalado ou no PATH.\n", saida_svg);
        }
        
        FILE *f = fopen(saida_mvs, "w");
        if (!f) {
            erro("Erro ao criar o arquivo de saida MVS.");
            return 1;
        }
        geracod(raiz, f);
        fclose(f);

        printf("Codigo MVS gerado em: %s\n", saida_mvs);
    } else {
        printf("Erros durante a analise sintatica.\n");
    }

    return 0;
}