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
	@bison -v -d $(BISON_SOURCE) -o sintatico.c
	@flex  -o lexico.c $(FLEX_SOURCE)
	@gcc -I. lexico.c sintatico.c $(C_SOURCES) -o simples


simples.exe: $(C_SOURCES) $(BISON_SOURCE) $(FLEX_SOURCE)
	@bison -v -d $(BISON_SOURCE) -o sintatico.c
	@flex  -o lexico.c $(FLEX_SOURCE)
	@gcc -I. lexico.c sintatico.c $(C_SOURCES) -o simples.exe


ifeq ($(OS),Windows_NT)
    RM = del /F /Q
else
    RM = rm -f
endif

limpa:
	@echo "Limpando..."
	@$(RM) lexico.c
	@$(RM) sintatico.c
	@$(RM) sintatico.h
	@$(RM) sintatico.output
	@$(RM) simples
	@$(RM) simples.exe
	@$(RM) *.dot
	@$(RM) *.svg
	@$(RM) *.mvs

