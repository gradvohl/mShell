# Arquivo Makefile para compilar o esqueleto do programa mshell.
#
# Criado por Prof. Andre Leon S. Gradvohl, Dr.
# e-mail: gradvohl@ft.unicamp.br
#
# Ultima versao: Qui 8 Out 11:39:48 -03 2024
#
#
# Compilador padrao
CC=gcc
# 
# Bibliotecas
FLAGS=-Wall -Wextra -pedantic -std=c99
#
# Arquivos fonte
FONTES=mshell.c funcoes.c
#
# Arquivos objeto
OBJETOS=$(FONTES:.c=.o)
#
# Diretorio onde o programa sera gerado
DIRETORIOPROGRAMA=.
#
# Nome do programa
EXECUTAVEL=mshell

#Dependencias de Compilacao
all: $(EXECUTAVEL)

$(EXECUTAVEL): $(OBJETOS)
	$(CC) $(FLAGS) $(OBJETOS) -o $(DIRETORIOPROGRAMA)/$(EXECUTAVEL)

mshell.o: mshell.c funcoes.h
	$(CC) $(FLAGS) -c mshell.c -o mshell.o

funcoes.o: funcoes.c funcoes.h
	$(CC) $(FLAGS) -c funcoes.c -o funcoes.o

#Limpeza
clean:
	rm -f $(OBJETOS) $(EXECUTAVEL)