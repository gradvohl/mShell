# mShell - Mini Shell
![Linguagem C](https://img.shields.io/badge/Linguagem-C-blue?style=plastic&logo=c)
![Makefile](https://img.shields.io/badge/Build-Makefile-orange?style=plastic&logo=gnu)
![Licença GPLv3](https://img.shields.io/badge/Licen%C3%A7a-GPLv3-brightgreen?style=plastic)
![Status](https://img.shields.io/badge/Status-Em%20desenvolvimento-yellow?style=plastic)



Disciplina: TT304 - Sistemas Operacionais
Instituição: Faculdade de Tecnologia (FT) - UNICAMP
Docente: Prof. Dr. André Leon Sampaio Gradvohl

Este repositório contém o esqueleto base para o desenvolvimento do projeto prático da disciplina TT304. O objetivo é implementar um mini interpretador de comandos (mShell) em linguagem C, utilizando chamadas de sistema POSIX/Linux para gerenciamento de processos, comunicação interprocessos, redirecionamento de descritores de arquivos e tratamento de sinais.

## :clipboard: Descrição do Projeto

O mShell é um shell simplificado que opera em um laço REPL (Read-Eval-Print Loop): lê uma linha de comando, interpreta os argumentos, executa o comando e exibe o resultado, repetindo o ciclo até que o usuário digite exit ou envie EOF (Ctrl+D).

O esqueleto fornecido já implementa a estrutura básica do REPL, o parser inicial com strtok() e os protótipos das funções principais. As lacunas marcadas com TODO devem ser preenchidas pelos estudantes com as chamadas de sistema adequadas.

### Funcionalidades a Implementar
| Funcionalidade	| Descrição	| Chamadas de sistema sugeridas |
| --------------  | --------- | ----------------------------- |
|Comandos embutidos| cd, pwd, exit executados no próprio shell (sem fork) | ``chdir(), getcwd(), getenv()`` |
|Comandos externos	|Execução de programas do PATH via fork() + execvp()   |	``fork(), execvp(), waitpid()`` |
|Execução em background	| Comandos terminados com & não bloqueiam o prompt |	``fork(), waitpid()`` com WNOHANG
|Redirecionamento de E/S |	Operadores < (entrada) e > (saída) |	``open(), dup2(), close()`` |
|Pipes	| Encadeamento de dois comandos com | ``pipe(), fork(), dup2()``|
|Tratamento de sinais	| Ignorar SIGINT no shell pai; tratar SIGCHLD	| ``signal() ou sigaction()``|

## :file_folder: Estrutura do Repositório
```text
.
├── mshell.c          # Função main e laço REPL
├── funcoes.c         # Implementação das funções auxiliares
├── funcoes.h         # Protótipos e constantes globais
├── makefile.txt      # Arquivo Makefile para compilação
└── README.md         # Este arquivo
```

## Arquivos Fornecidos
- ``mshell.c``: Contém a função main() com o laço REPL, a leitura da linha, o parsing e a chamada das funções de execução.
- ``funcoes.h``: Declara as constantes (``MAX_LINE``, ``MAX_ARGS``, ``DELIMITERS``) e os protótipos das funções que você deve implementar.
- ``funcoes.c``: Contém o esqueleto das funções com comentários detalhados e marcações TODO.
- ``makefile``: _Makefile_ para compilar o projeto. 

## :wrench: Compilação
### Pré-requisitos
- Sistema operacional Linux (ou ambiente POSIX compatível)
- Compilador GCC
- Ferramenta make

### Compilação 
```bash
gcc -Wall -Wextra -pedantic -std=c99 mshell.c funcoes.c -o mshell
```

:warning: Atenção: O código deve compilar sem warnings. A presença de advertências pode acarretar desconto de 3 pontos na avaliação.

▶️ Execução
```bash
./mshell
```

O prompt será exibido no formato:
```text
mShell:/caminho/atual>
```

### Exemplos de Uso
```bash
# Comando externo simples
mShell:/home/user> ls -la

# Comando embutido
mShell:/home/user> cd /tmp
mShell:/tmp> pwd

# Redirecionamento de saída
mShell:/tmp> echo "hello" > saida.txt

# Redirecionamento de entrada
mShell:/tmp> cat < saida.txt

# Execução em background
mShell:/tmp> sleep 10 &

# Pipe (bônus)
mShell:/tmp> cat arquivo.txt | grep "palavra"
```

## 📝Tarefas a Implementar

Todas as lacunas estão marcadas com TODO no arquivo funcoes.c. Abaixo está um resumo do que deve ser feito em cada função:

1. ```handle_builtin()```
- ```cd```: Se args[1] for NULL, usar getenv("HOME"); caso contrário, usar args[1]. Chamar chdir(). Em caso de erro, usar perror().
- ```pwd```: Usar getcwd() para obter o diretório atual e imprimir na saída padrão.

2. execute_command()
- Criar processo filho com ```fork()```.
- No filho:
  - Se input_file != NULL, abrir com open(input_file, O_RDONLY) e redirecionar stdin com dup2().
  - Se output_file != NULL, abrir com open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644) e redirecionar stdout com dup2().
  - Executar execvp(args[0], args). Em caso de erro, usar perror() e exit(EXIT_FAILURE).
    
- No pai:
     - Se background == 0, aguardar com waitpid().
     - Se background == 1, imprimir o PID do filho e continuar.

3. setup_signal_handlers()
  - Ignorar SIGINT no shell pai (para que Ctrl+C não encerre o shell).

  - Registrar sigchld_handler para SIGCHLD (para recolher processos zumbis).

4. execute_pipe() (Bônus)
  - Criar um pipe com pipe(pipefd).
  - Criar dois processos filhos (um para cada comando).
  - Redirecionar stdout do primeiro filho para pipefd[1] e stdin do segundo filho para pipefd[0].
  - Fechar descritores no pai e aguardar os filhos (se não for background).

## ⚠️ Restrições e Boas Práticas
- Proibição da função system(): É estritamente proibido usar system() da stdlib.h. O gerenciamento de processos deve ser feito manualmente com fork() + execvp().
- Gerenciamento de memória: Toda memória alocada dinamicamente deve ser liberada com free(). O parser fornecido não aloca memória, mas se você adicionar alocações (ex.: para pipes), lembre-se de desalocar.
- Tratamento de erros: Todas as chamadas de sistema devem verificar o valor de retorno e exibir mensagens adequadas com perror() ou fprintf(stderr, ...).
- Compilação sem warnings: O código deve compilar limpo com as flags -Wall -Wextra -pedantic -std=c99.

## 📚 Recursos e Referências
- Man pages: man 2 fork, man 2 execvp, man 2 dup2, man 2 pipe, man 2 waitpid, man 2 chdir, man 2 open, man 2 signal
- Livro: Sistemas Operacionais Modernos - Andrew S. Tanenbaum
- Documentação POSIX: pubs.opengroup.org

## 📦 Entregas do Projeto
Os produtos a serem entregues são:
- Código-fonte completo, documentado e pronto para compilação em Linux (em um repositório).
- Relatório em PDF, contendo: descrição do problema, instruções de compilação, estrutura da solução, justificativas das decisões e conclusões. O relatório deve incluir o link deste repositório e o link do vídeo de demonstração.
- Vídeo demonstrando o funcionamento do programa, explicando a lógica, mostrando o código, a compilação e execuções de teste.

**Importante: A ausência do vídeo ou do link do repositório no relatório implica nota zero no projeto.**

## :busts_in_silhouette: Uso Responsável de IA

A utilização de ferramentas de Inteligência Artificial é permitida apenas para revisão gramatical, ortográfica, coesão e melhoria da qualidade textual do relatório. O uso de IA para gerar código ou resolver o projeto não é permitido. Consulte as orientações oficiais da UNICAMP e preencha a Declaração de Uso de IA disponível no Moodle.

Boa sorte no desenvolvimento do mShell! 🚀
