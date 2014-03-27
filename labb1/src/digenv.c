/*TODO: Add description */

#include <sys/types.h> /* definierar typen pid_t */
#include <errno.h> /* definierar bland annat perror() */
#include <stdio.h> /* definierar bland annat stderr */
#include <stdlib.h> /* definierar bland annat rand() och RAND_MAX */
#include <unistd.h> /* definierar bland annat pipe() */
#include <string.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

int main(int argc, char **argv, char **envp) {
  int stdin_pipe = STDIN_FILENO;
  int stdout_pipe[2];
  pid_t childpid = -1;

  int step;
  for(step = 3; step >= 0; step--) {
    /*Skip grep step if no arguments*/
    if(argc == 1 && step == 2) { step--; }

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
        if(dup2(stdin_pipe, STDIN_FILENO) < 0) { perror("Error redirecting inpipe to STDIN"); exit(1); }
      }

      /*Redirect stdout to write pipe */
      if(step > 0) {
        if(dup2(stdout_pipe[PIPE_WRITE], STDOUT_FILENO) < 0) { perror("Error redirecting STDOUT to outpipe"); exit(1); }
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
          perror("Error executing less");
          break;
        default:
          break;
      }
      exit(1); /*Will only be reached on error*/

    /*Parent process execution*/
    } else {
      if(step > 0) {
        /*Close stdout write pipe*/
        if(close(stdout_pipe[PIPE_WRITE]) < 0) { perror("Error closing stdout read pipe in parent"); exit(1); }
      }

      /*Connect out pipe to next in pipe*/
      stdin_pipe = stdout_pipe[PIPE_READ];
    }
  }

  /*Wait for all children to finish*/
  while(wait() >= 0) {};

  return 0;
}