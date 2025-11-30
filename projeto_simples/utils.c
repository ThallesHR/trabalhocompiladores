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
#include <stdarg.h>

/* Função genérica para exibir mensagens de erro fatal e encerrar o programa imediatamente. */
void erro(const char *msg) {
    fprintf(stderr, "Erro Fatal: %s\n", msg);
    exit(1);
}