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
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
ptno raiz;


extern int yylex();
extern void erro(const char *msg); 
void yyerror(const char *s);
%}


%union {
    int val;
    char *str;
    struct no *no;
}


%token PROGRAMA INICIO FIM INTEIRO LEIA ESCREVA
%token <val> NUM
%token <str> ID


%type <no> programa comandos comando expr termo fator


%left '+' '-'
%left '*' '/'

%%


programa:
    PROGRAMA ID INICIO comandos FIM {
        ptno p = criaNo(N_PROGRAM, 0);
        adicionaFilho(p, criaNoStr(N_ID, $2));
        adicionaFilho(p, $4);
        raiz = p;
        $$ = p;
    }
;


comandos:
            { $$ = criaNo(N_LISTA, 0); }
    | comandos comando  { adicionaFilho($1, $2); $$ = $1; }
;


comando:
      ID '=' expr ';' {
            ptno n = criaNo(N_ATR, 0);
            adicionaFilho(n, criaNoStr(N_ID, $1));
            adicionaFilho(n, $3);
            $$ = n;
      }
    | LEIA '(' ID ')' ';' {
            ptno n = criaNo(N_LEIA, 0);
            adicionaFilho(n, criaNoStr(N_ID, $3));
            $$ = n;
      }
    | ESCREVA '(' expr ')' ';' {
            ptno n = criaNo(N_ESCREVA, 0);
            adicionaFilho(n, $3);
            $$ = n;
      }
;

expr:
      expr '+' termo {
            ptno n = criaNo(N_PLUS, 0);
            adicionaFilho(n, $1);
            adicionaFilho(n, $3);
            $$ = n;
      }
    | expr '-' termo {
            ptno n = criaNo(N_MINUS, 0);
            adicionaFilho(n, $1);
            adicionaFilho(n, $3);
            $$ = n;
      }
    | termo { $$ = $1; }
;

termo:
      termo '*' fator {
            ptno n = criaNo(N_MUL, 0);
            adicionaFilho(n, $1);
            adicionaFilho(n, $3);
            $$ = n;
      }
    | termo '/' fator {
            ptno n = criaNo(N_DIV, 0);
            adicionaFilho(n, $1);
            adicionaFilho(n, $3);
            $$ = n;
      }
    | fator { $$ = $1; }
;

fator:
      NUM {
            $$ = criaNo(N_NUM, $1);
      }
    | ID {
            $$ = criaNoStr(N_ID, $1);
      }
    | '(' expr ')' {
            $$ = $2;
      }
;

%%


void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico: %s\n", s);
}