/*
 *
 * NAME: digenv
 *
 * SYNTAX:
 *   digenv [grep params]
 *
 * DECRIPTION:
 *   Digenv will list all environment variables optionally filtered
 *   through grep, sorted and displayed in the default pager.
 *   The default pager is determined by consulting the PAGER
 *   environment variable. If it doesn't exist, less and then more
 *   will be tried. The program is equivalent to running
 *   printenv | grep pattern | sort | less
 *
 * OPTIONS:
 *   grep params     The optional parameters to pass to grep
 *
 * SEE ALSO:
 *   grep(1)
 *
 * EXAMPLES:
 *   digenv
 *   digenv PATH
 *   digenv -v HOME
 *
 * AUTHOR:
 *   Carl Svensson <carlsven@kth.se>
 */

/* digenv
 *
 * The program runs a loop. Each step forks a new process
 * which executes one step of the program. At each loop
 * the main program connects the output pipe of one child
 * process with the input pipe of the next process except
 * for the last step where the output is kept to stdout.
 * Finally the parent program waits for all children to
 * terminate.
 */

#include <sys/types.h> /* definierar typen pid_t */
#include <sys/wait.h> /* definerar wait() */
#include <errno.h> /* definierar bland annat perror() */
#include <stdio.h> /* definierar bland annat stderr */
#include <stdlib.h> /* definierar bland annat rand() och RAND_MAX */
#include <unistd.h> /* definierar bland annat pipe() */
#include <string.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

/* main
 *
 * main runs the digenv procedure and returns the exit code
 */
int main(
  int     argc, /* The number of command line arguments */
  char  **argv, /* The array of command line arguments  */
  char  **envp) /* The array of envirnment variables    */
{

  int stdin_pipe = STDIN_FILENO;
  int stdout_pipe[2];
  pid_t childpid = -1;

  int step;
  for(step = 3; step >= 0; step--) {

    /*
     * Skip grep step if no arguments 
     *
     */
    if(argc == 1 && step == 2) { step--; }

    /* 
     * Create pipe
     *
     */
    if(step > 0) {
      if(pipe(stdout_pipe) < 0) { perror("Error creating stdout pipe"); exit(1); }
    }

    /*
     * Fork process 
     *
     */
    if((childpid = fork()) < 0) {
      close(stdout_pipe[PIPE_WRITE]);
      close(stdout_pipe[PIPE_READ]);
      perror("Error forking"); exit(1);
    }

    /*
     * Child process execution 
     *
     */
    if(childpid == 0) {

      /*
       * Redirect read pipe to stdin
       *
       */
      if(stdin_pipe != STDIN_FILENO) {
        if(dup2(stdin_pipe, STDIN_FILENO) < 0) { perror("Error redirecting inpipe to STDIN"); exit(1); }
      }

      /* 
       * Redirect stdout to write pipe
       *
       */
      if(step > 0) {
        if(dup2(stdout_pipe[PIPE_WRITE], STDOUT_FILENO) < 0) { perror("Error redirecting STDOUT to outpipe"); exit(1); }
        if(close(stdout_pipe[PIPE_READ]) < 0) { perror("Error closing read pipe in child"); exit(1); }
      } 

      /*
       * Perform work
       *
       */
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
          if(getenv("PAGER") != NULL) {
            execlp(getenv("PAGER"), getenv("PAGER"), NULL);  
          }

          execlp("less", "less", NULL);
          execlp("more", "more", NULL);
          perror("Error executing pager");
          break;
        default:
          break;
      }

      /*
       * Will only be reached on error
       *
       */
      exit(1);

    /*
     * Parent process execution
     *
     */
    } else {
      if(step > 0) {

        /*
         * Close stdout write pipe
         *
         */
        if(close(stdout_pipe[PIPE_WRITE]) < 0) { perror("Error closing stdout read pipe in parent"); exit(1); }
      }

      /*
       * Connect out pipe to next in pipe
       *
       */
      stdin_pipe = stdout_pipe[PIPE_READ];
    }
  }

  /*
   * Wait for all children to finish
   *
   */
  while(wait(NULL) >= 0) {};

  return 0;
}