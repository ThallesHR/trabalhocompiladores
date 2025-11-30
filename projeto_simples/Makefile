#=============================================================
# UNIFAL = Universidade Federal de Alfenas.
# BACHARELADO EM CIENCIA DA COMPUTACAO.
# Trabalho . . : Construcao Arvore Sintatica e Geracao de Codigo
# Disciplina : Teoria de Linguagens e Compiladores
# Professor . : Luiz Eduardo da Silva
# Aluno . . . . . : Thalles Henrique Gonzaga Rosa Pereira
# Data . . . . . . : 20/11/2025
#=============================================================

simples: utils.c tree.c lexico.l sintatico.y
	@echo "Compilando..."
	@flex -o lexico.c lexico.l
	@bison -v -d sintatico.y -o sintatico.c
	@gcc lexico.c sintatico.c utils.c tree.c -o simples

limpa:
	@echo "Limpando..."
	@rm -f lexico.c sintatico.c sintatico.h sintatico.output simples simples.exe *.dot *.svg *.mvs