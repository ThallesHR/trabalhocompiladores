/*+=============================================================
| UNIFAL = Universidade Federal de Alfenas.
| BACHARELADO EM CIENCIA DA COMPUTACAO.
| Trabalho . . : Construcao Arvore Sintatica e Geracao de Codigo
| Disciplina : Teoria de Linguagens e Compiladores
| Professor . : Luiz Eduardo da Silva
| Aluno . . . . . : Thalles Henrique Gonzaga Rosa Pereira
| Data . . . . . . : 20/11/2025
+=============================================================*/
%{
#include "sintatico.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%%

"programa"      { return PROGRAMA; }
"inicio"        { return INICIO; }
"fim"           { return FIM; }
"inteiro"       { return INTEIRO; }
"leia"          { return LEIA; }
"escreva"       { return ESCREVA; }

[0-9]+          { yylval.val = atoi(yytext); return NUM; }

[a-zA-Z][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return ID;
}

[ \t\r\n]+      ;

.               { return yytext[0]; }

%%

int yywrap() { return 1; }
