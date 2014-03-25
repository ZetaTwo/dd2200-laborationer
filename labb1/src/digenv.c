#include <sys/types.h> /* definierar typen pid_t */
#include <errno.h> /* definierar bland annat perror() */
#include <stdio.h> /* definierar bland annat stderr */
#include <stdlib.h> /* definierar bland annat rand() och RAND_MAX */
#include <unistd.h> /* definierar bland annat pipe() */
#include <string.h>

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 4096

int perform_step(int step, int in_pipe, char **envp);

int main(int argc, char **argv, char **envp) {
  /*Start recursion on first step*/
  return perform_step(3, 0, envp);
}

int perform_step(int step, int in_pipe, char **envp) {
  int return_value = 0;
  int pipe_filedesc[2];
  int childpid = 0;

  if(step > 0) { /*Don't fork on last step*/
    /*Open pipes*/
    return_value = pipe(pipe_filedesc);
    if(return_value == -1) { fprintf(stderr, "Error opening pipe.\n"); /*TODO: Error*/ }

    childpid = fork(); /*Fork process*/
    if(childpid == -1) { fprintf(stderr, "Error forking in step %d.\n", step); /*TODO: Error*/ }

    if(childpid == 0) { /*Runs in child process*/

      /*Close write pipe*/
      return_value = close(pipe_filedesc[WRITE]);
      if(return_value == -1) { fprintf(stderr, "Error closing write pipe in step %d.\n", step); /*TODO: Error*/ }

      /*Recurse*/
      return perform_step(step-1, pipe_filedesc[READ], envp);

    } else { /*Runs in parent process*/

      /*Close read pipe*/
      return_value = close(pipe_filedesc[READ]);
      if(return_value == -1) { fprintf(stderr, "Error closing read pipe in step %d.\n", step); /*TODO: Error*/ }
    }
  }

  if(childpid != 0 || step == 0) {

    char buffer[BUFFER_SIZE];
    size_t readlen = 0;
    size_t writelen = 0;

    switch(step) {
      case 3:
        for(; *envp != NULL; envp++) {
          size_t envplen = strlen(*envp);
          return_value = write(pipe_filedesc[WRITE], *envp, envplen);
          if(return_value == -1) { fprintf(stderr, "Error writing to child process in step %d.\n", step); /*TODO: Error*/ }
          return_value = write(pipe_filedesc[WRITE], "\n", 1);
          if(return_value == -1) { fprintf(stderr, "Error writing to child process in step %d.\n", step); /*TODO: Error*/ }
        }  
      break;

      case 0:
        
        while((readlen = read(in_pipe, buffer, BUFFER_SIZE)) > 0) {
          printf(buffer);          
        }

        if(readlen == -1) { fprintf(stderr, "Error reading from parent process in step %d.\n", step); /*TODO: Error*/ }
      break;
      default:       
        while((readlen = read(in_pipe, buffer, BUFFER_SIZE)) > 0) {
          writelen = write(pipe_filedesc[WRITE], buffer, BUFFER_SIZE);
          if(writelen == -1 ) { fprintf(stderr, "Error writing to child process in step %d.\n", step); /*TODO: Error*/ };
        }
       
        if(readlen == -1) { fprintf(stderr, "Error reading from parent process in step %d.\n", step); /*TODO: Error*/ }
      break;
    }
  }

  /*Done writing, close pipe*/
  return_value = close(pipe_filedesc[WRITE]);
  if(return_value == -1) { fprintf(stderr, "Error closing write pipe in step %d.\n", step); /*TODO: Error*/ }

  /*If we have children, wait for them*/
  if(step > 0) {
    return_value = wait();
    if(return_value == -1) { fprintf(stderr, "Error waiting for child process in step %d.\n", step); /*TODO: Error*/ }
  }
}