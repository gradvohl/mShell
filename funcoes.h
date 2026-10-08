/* Constantes Globais */
#define MAX_LINE 1024     /* Tamanho máximo de uma linha de comando */
#define MAX_ARGS 64       /* Número máximo de argumentos de um comando */
#define DELIMITERS " \t\r\n\a" /* Delimitadores para quebra de tokens */

/**
 * Exibe o prompt de comando customizado na saída padrão (stdout).
 */
void print_prompt(void);

/**
 *  Realiza a análise léxica (parsing) da linha de comando digitada
 * pelo usuário.*/
int parse_line(char *, char **, int *, char **, char **);

/**
 * Verifica e executa comandos embutidos (built-in commands) do shell.
 */
int handle_builtin(char **);

/**
 * Cria um processo filho e executa um comando externo via execvp.
 */
void execute_command(char **, int, char *, char *);

/**
 * Executa dois comandos interligados por uma tubulação anônima (pipe '|').
 */
void execute_pipe(char **, char **, int);

/**
 * Configura os tratadores de sinais (signal handlers) do ambiente do shell.
 */
void setup_signal_handlers(void);

/**
 * Manipulador (handler) para o sinal SIGCHLD.
 */ 
void sigchld_handler(int);
