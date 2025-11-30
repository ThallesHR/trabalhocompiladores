#=============================================================
# UNIFAL = Universidade Federal de Alfenas.
# BACHARELADO EM CIENCIA DA COMPUTACAO.
# Trabalho . . : Construcao Arvore Sintatica e Geracao de Codigo
# Disciplina : Teoria de Linguagens e Compiladores
# Professor . : Luiz Eduardo da Silva
# Aluno . . . . . : Thalles Henrique Gonzaga Rosa Pereira
# Data . . . . . . : 20/11/2025
#=============================================================

C_SOURCES = utils.c tree.c geracod.c main.c
BISON_SOURCE = sintatico.y
FLEX_SOURCE = lexico.l

simples: $(C_SOURCES) $(BISON_SOURCE) $(FLEX_SOURCE)
	bison -v -d $(BISON_SOURCE) -o sintatico.c
	flex -o lexico.c $(FLEX_SOURCE)
	gcc -I. lexico.c sintatico.c $(C_SOURCES) -o simples

limpa:
	rm -f lexico.c sintatico.c sintatico.h sintatico.output simples simples.exe *.dot *.svg *.mvs