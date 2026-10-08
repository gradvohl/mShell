#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "funcoes.h"

/**
 * @brief Exibe o prompt de comando customizado na saída padrão (stdout).
 * 
 * Obtém o diretório de trabalho atual usando a função getcwd() 
 * e formata o prompt.
 */
void print_prompt(void) 
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("mShell:%s> ", cwd);
    } else {
        printf("mShell> ");
    }
    fflush(stdout);
}


/**
 * @brief Realiza a análise léxica (parsing) da linha de comando digitada 
 * pelo usuário.
 * 
 * Divide a string de entrada em tokens e identifica operadores de controle 
 * de execução em background ('&') e redirecionamento de E/S ('<' e '>').
 * 
 * @param[in]  line  Ponteiro para a string com a linha de comando bruta lida do teclado.
 * @param[out] args  Vetor de ponteiros para strings onde serão armazenados os argumentos extraídos.
 * @param[out] background   Ponteiro para flag inteira que recebe 1 se o comando terminar com '&', ou 0 caso contrário.
 * @param[out] input_file   Ponteiro para ponteiro de char que receberá o nome do arquivo de entrada (<), se houver.
 * @param[out] output_file  Ponteiro para ponteiro de char que receberá o nome do arquivo de saída (>), se houver.
 * 
 * @return Retorna o número total de argumentos inseridos no vetor `args`.
 */
int parse_line(char *line, char **args, int *background, char **input_file, char **output_file) 
{
    int i = 0;
    char *token = strtok(line, DELIMITERS);

    while (token != NULL && i < MAX_ARGS - 1) {
        if (strcmp(token, "&") == 0) {
            *background = 1;
        } else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, DELIMITERS);
            if (token != NULL) {
                *input_file = token;
            } else {
                fprintf(stderr, "mShell: erro de sintaxe próximo a '<'\n");
            }
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, DELIMITERS);
            if (token != NULL) {
                *output_file = token;
            } else {
                fprintf(stderr, "mShell: erro de sintaxe próximo a '>'\n");
            }
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, DELIMITERS);
    }
    args[i] = NULL; /* O último elemento deve ser NULL para o execvp */
    return i;
}

/**
 * @brief Verifica e executa comandos embutidos (built-in commands) do shell.
 * 
 * Comandos embutidos são executados no próprio processo do shell sem a criação
 * de processos filhos via fork() (ex.: `cd`, `pwd`, `exit`).
 * 
 * @param[in] args Vetor de argumentos contendo o comando e seus parâmetros (terminado em NULL).
 * 
 * @return Retorna 1 se o comando informado for um built-in processado, ou 0 caso seja um comando externo.
 */
int handle_builtin(char **args) 
{
    if (args[0] == NULL) return 0;

    /* Comando 'exit' */
    if (strcmp(args[0], "exit") == 0) {
        printf("Encerrando o mShell...\n");
        exit(EXIT_SUCCESS);
    }

    /* Comando 'cd' */
    if (strcmp(args[0], "cd") == 0) {
        /* TODO: Implementar a troca de diretório com chdir()
         * Dica: Se args[1] for NULL, ir para o diretório de HOME (getenv("HOME"))
         */
        printf("[TODO] Implementar o comando built-in 'cd'\n");
        return 1;
    }

    /* Comando 'pwd' */
    if (strcmp(args[0], "pwd") == 0) {
        /* TODO: Implementar a impressão do diretório corrente com getcwd() */
        printf("[TODO] Implementar o comando built-in 'pwd'\n");
        return 1;
    }

    return 0; /* Não é comando embutido */
}


/**
 * @brief Cria um processo filho e executa um comando externo via execvp.
 * 
 * Gerencia a criação do processo (fork), os redirecionamentos dos descritores
 * de arquivos padrão (stdin/stdout via dup2) e a execução em segundo plano.
 * 
 * @param[in] args         Vetor de argumentos do comando a ser executado (terminado em NULL).
 * @param[in] background   Flag indicando se o comando deve ser executado em background (1) ou foreground (0).
 * @param[in] input_file   Nome do arquivo para redirecionamento da entrada padrão (<), ou NULL se não houver.
 * @param[in] output_file  Nome do arquivo para redirecionamento da saída padrão (>), ou NULL se não houver.
 */
void execute_command(char **args, int background, char *input_file, char *output_file) 
{
    /* TODO: Implementar o fluxo de criação e execução do processo:
     * 1. Criar processo filho com fork()
     * 2. No processo FILHO:
     *    a) Se input_file != NULL, abrir o arquivo (open) e redirecionar stdin (dup2)
     *    b) Se output_file != NULL, abrir/criar o arquivo (open) e redirecionar stdout (dup2)
     *    c) Executar o comando com execvp(args[0], args)
     *    d) Tratar erros de execução (ex: comando não encontrado) com perror() e exit()
     * 3. No processo PAI:
     *    a) Se background == 0, aguardar a finalização do filho com waitpid()
     *    b) Se background == 1, imprimir o PID do processo filho em background e continuar
     */

    printf("[TODO] Executar comando externo: %s (Background: %d)\n", args[0], background);
    if (input_file) printf("       Entrada redirecionada de: %s\n", input_file);
    if (output_file) printf("       Saída redirecionada para: %s\n", output_file);
}

/**
 * @brief Executa dois comandos interligados por uma tubulação anônima (pipe '|').
 * 
 * Conecta a saída padrão (stdout) do primeiro comando à entrada padrão (stdin)
 * do segundo comando através da chamada de sistema pipe().
 * 
 * @param[in] args_left   Vetor de argumentos do comando à esquerda do pipe.
 * @param[in] args_right  Vetor de argumentos do comando à direita do pipe.
 * @param[in] background  Flag indicando se o encadeamento deve rodar em background (1 ou 0).
 */
void execute_pipe(char **args_left, char **args_right, int background) 
{
    (void)args_left;
    (void)args_right;
    (void)background;

    /* TODO (BÔNUS / AVANÇADO):
     * 1. Criar o pipe usando pipe(pipefd)
     * 2. Criar o primeiro filho para o comando da esquerda (args_left):
     *    - Redirecionar stdout para pipefd[1]
     * 3. Criar o segundo filho para o comando da direita (args_right):
     *    - Redirecionar stdin para pipefd[0]
     * 4. Fechar descritores no pai e aguardar o término dos processos (se não for background)
     */
    printf("[TODO] Implementar a execução encadeada por pipe '|'\n");
}

/**
 * @brief Configura os tratadores de sinais (signal handlers) do ambiente do shell.
 * 
 * Registra as rotinas para ignorar/tratar sinais como SIGINT (Ctrl+C) e SIGCHLD (limpeza de zumbis).
 */
void setup_signal_handlers(void) 
{
    /* TODO:
     * 1. Ignorar SIGINT (Ctrl+C) no shell pai ou tratar adequadamente.
     * 2. Tratar SIGCHLD para recolher automaticamente processos zumbis
     *    criados em background via waitpid(-1, NULL, WNOHANG).
     */
}

/**
 * @brief Manipulador (handler) para o sinal SIGCHLD.
 * 
 * Recolhe os status dos processos filhos terminados em segundo plano para
 * evitar o acúmulo de processos no estado zumbi (zombie processes).
 * 
 * @param[in] sig Número do sinal recebido (SIGCHLD).
 */
void sigchld_handler(int sig) 
{
    (void)sig;
    /* Trata os filhos terminados sem bloquear a execução */
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
