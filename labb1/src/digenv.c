#include <sys/types.h> /* definierar typen pid_t */
#include <errno.h> /* definierar bland annat perror() */
#include <stdio.h> /* definierar bland annat stderr */
#include <stdlib.h> /* definierar bland annat rand() och RAND_MAX */
#include <unistd.h> /* definierar bland annat pipe() */
#include <string.h>

#define PIPE_READ 0
#define PIPE_WRITE 1
#define BUFFER_SIZE 4096

int perform_step(int step, int stdin_pipe, int argc, char **argv, char **envp);

int main(int argc, char **argv, char **envp) {
  perform_step(3, STDIN_FILENO, argc, argv, envp);
  return 0;
}

int perform_step(int step, int stdin_pipe, int argc, char **argv, char **envp) {
  int stdout_pipe[2];
  pid_t childpid = -1;

  /*Create pipe*/
  if(step > 0) {
    if(pipe(stdout_pipe) < 0) { perror("Error creating stdout pipe"); exit(1); }
  }

  /*Fork process*/
  if((childpid = fork()) < 0) {
    close(stdout_pipe[PIPE_WRITE]);
    close(stdout_pipe[PIPE_READ]);
    perror("Error forking"); exit(1);
  }

  /*Child process execution*/
  if(childpid == 0) {

    /*Redirect read pipe to stdin*/
    if(stdin_pipe != STDIN_FILENO) {
      if(dup2(stdin_pipe, STDIN_FILENO) < 0) { perror("Error readirecting inpipe to STDIN"); exit(1); }
    }

    /*Redirect stdout to write pipe */
    if(step > 0) {
      if(dup2(stdout_pipe[PIPE_WRITE], STDOUT_FILENO) < 0) { perror("Error readirecting STDOUT to outpipe"); exit(1); }
      if(close(stdout_pipe[PIPE_READ]) < 0) { perror("Error closing read pipe in child"); exit(1); }
    } 

    /*Perform work*/
    switch(step) {
      case 3:
        execlp("printenv", "printenv", NULL);
        perror("Error executing printenv");
        break;
      case 2:
        execvp("grep", argv);
        perror("Error executing grep");
        break;
      case 1:
        execlp("sort", "sort", NULL);
        perror("Error executing sort");
        break;
      case 0:
        execlp("less", "less", NULL);
        perror("Error executing sort");
        break;
      default:
        break;
    }
    exit(1);

  /*Parent process execution*/
  } else {
    
    if(step > 0) {

      /*Close stdout write pipe*/
      if(close(stdout_pipe[PIPE_WRITE]) < 0) { perror("Error closing stdout read pipe in parent"); exit(1); }

      /*Perform next step*/
      if(argc == 1 && step == 3) { step--; }
      perform_step(step-1, stdout_pipe[PIPE_READ], argc, argv, envp);
    }

    wait();
  }

  return 0;
}