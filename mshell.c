/**
 * ============================================================================
 * UNICAMP - Universidade Estadual de Campinas
 * Faculdade de Tecnologia (FT)
 * Disciplina: TT304 - Sistemas Operacionais
 * Docente: Prof. Dr. André Leon Sampaio Gradvohl
 * ============================================================================
 * TRABALHO PRÁTICO: Mini-Shell (mShell)
 * 
 * Arquivo: mshell.c
 * Descrição: Esqueleto base para o desenvolvimento do interpretador de comandos.
 * 
 * Compilação sugerida:
 *   gcc -Wall -Wextra -pedantic -std=c99 mshell_template.c -o mshell
 * ============================================================================
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "funcoes.h"

/**
 * Função Principal (REPL)
 */
int main(void) {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    int background = 0;
    char *input_file = NULL;
    char *output_file = NULL;

    /* Configura os manipuladores de sinais (SIGINT, SIGCHLD) */
    setup_signal_handlers();

    while (1) {
        /* 1. Imprime o prompt */
        print_prompt();

        /* 2. Lê a linha de comando enviada pelo usuário */
        if (fgets(line, sizeof(line), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\nSaindo do mShell...\n");
                exit(EXIT_SUCCESS);
            }
            perror("Erro ao ler comando");
            continue;
        }

        /* 3. Faz o parsing da linha lida */
        background = 0;
        input_file = NULL;
        output_file = NULL;
        
        int status_parse = parse_line(line, args, &background, &input_file, &output_file);
        
        /* Se a linha estiver vazia ou for um comentário, continua */
        if (status_parse == 0 || args[0] == NULL) {
            continue;
        }

        /* 4. Verifica e executa comandos embutidos (built-ins: cd, pwd, exit) */
        if (handle_builtin(args)) {
            continue;
        }

        /* 5. Executa comandos externos (com suporte a E/S e background) */
        /* TODO: Se houver detecção de pipe '|', chamar execute_pipe() */
        execute_command(args, background, input_file, output_file);
    }

    return 0;
}
