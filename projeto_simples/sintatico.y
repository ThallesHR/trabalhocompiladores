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

/* --- TOKENS NOVOS ADICIONADOS AQUI --- */
%token PROGRAMA INICIO FIM INTEIRO LEIA ESCREVA
%token SE ENTAO SENAO FIMSE ENQUANTO FACA FIMENQUANTO
%token MAIOR MENOR IGUAL DIFERENTE MAIG MENIG
%token <val> NUM
%token <str> ID

%type <no> programa declaracoes lista_vars comandos comando expr termo fator condicao

%left '+' '-'
%left '*' '/'

%%

/* --- Regras da Gramática --- */

programa:
    PROGRAMA ID declaracoes INICIO comandos FIM {
        ptno p = criaNo(N_PROGRAM, 0);
        adicionaFilho(p, criaNoStr(N_ID, $2));
        adicionaFilho(p, $3); 
        adicionaFilho(p, $5); 
        raiz = p;
        $$ = p;
    }
;

declaracoes:
      /* vazio */ { $$ = NULL; }
    | INTEIRO lista_vars { 
        ptno n = criaNo(N_DECL_VAR, 0);
        adicionaFilho(n, $2);
        $$ = n;
    }
;

lista_vars:
      ID { $$ = criaNoStr(N_ID, $1); }
    | lista_vars ID { 
          adicionaFilho($1, criaNoStr(N_ID, $2)); 
          $$ = $1; 
      }
;

comandos:
      /* vazio */       { $$ = criaNo(N_LISTA, 0); }
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
    /* --- REGRAS DO SE e ENQUANTO QUE FALTAVAM --- */
    | SE condicao ENTAO comandos FIMSE {
            ptno n = criaNo(N_SE, 0);
            adicionaFilho(n, $2);
            adicionaFilho(n, $4);
            $$ = n;
    }
    | SE condicao ENTAO comandos SENAO comandos FIMSE {
            ptno n = criaNo(N_SE, 0);
            adicionaFilho(n, $2);
            adicionaFilho(n, $4);
            adicionaFilho(n, $6);
            $$ = n;
    }
    | ENQUANTO condicao FACA comandos FIMENQUANTO {
            ptno n = criaNo(N_ENQUANTO, 0);
            adicionaFilho(n, $2);
            adicionaFilho(n, $4);
            $$ = n;
    }
;

condicao:
      expr MAIOR expr     { ptno n = criaNo(N_MAIOR, 0); adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | expr MENOR expr     { ptno n = criaNo(N_MENOR, 0); adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | expr IGUAL expr     { ptno n = criaNo(N_IGUAL, 0); adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | expr DIFERENTE expr { ptno n = criaNo(N_DIF, 0);   adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | expr MAIG expr      { ptno n = criaNo(N_MAIG, 0);  adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | expr MENIG expr     { ptno n = criaNo(N_MENIG, 0); adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | '(' condicao ')'    { $$ = $2; }
;

expr:
      expr '+' termo { ptno n = criaNo(N_PLUS, 0); adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | expr '-' termo { ptno n = criaNo(N_MINUS, 0); adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | termo { $$ = $1; }
;

termo:
      termo '*' fator { ptno n = criaNo(N_MUL, 0); adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | termo '/' fator { ptno n = criaNo(N_DIV, 0); adicionaFilho(n, $1); adicionaFilho(n, $3); $$ = n; }
    | fator { $$ = $1; }
;

fator:
      NUM { $$ = criaNo(N_NUM, $1); }
    | ID { $$ = criaNoStr(N_ID, $1); }
    | '(' expr ')' { $$ = $2; }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico: %s\n", s);
}