#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include "../include/timer.h"
#include "../include/command.h"
#include "../include/processlist.h"

#define STATISTICS

void install_signal_handler(struct sigaction *old);
void restore_signal_handler(struct sigaction *old);
void signal_handler(int, siginfo_t *, void *);

int execute_command(struct command *cmd, struct processlist *processes);
int change_working_directory(struct command *cmd);

int main(
  int     argc, /* The number of command line arguments */
  char  **argv, /* The array of command line arguments  */
  char  **envp) /* The array of envirnment variables    */
{
  /* Allocate command string variables */
  struct command command;
  init_command(&command);

  /* Allocate process list */
  struct processlist processes;
  init_processlist(&processes);

  /* Current working directory */
  char cwd[PATH_MAX];

  while(1) {
    /* Print prompt */
    if(getcwd(cwd, PATH_MAX) == NULL) { perror("Failed to get current working directory"); exit(1); }
    printf("%s> ", cwd);

    /* Get command */
    read_command(&command);
    parse_command(&command);

    /* Execute command */
    if(command.len_tokens > 1) {
      if(strcmp("exit", command.tokens[0]) == 0) {
        break; /* Exit program */

      } else if(strcmp("cd", command.tokens[0]) == 0) {
        change_working_directory(&command);

      } else {
        execute_command(&command, &processes);
      }
    

      /* Check for terminated background processes */
      /* Keep track of previous and current process in list when iterating */
      struct process *prev = &processes.start;
      struct process *current = prev->next;

      /* Iterate over whole list */
      while(current != &processes.end) {
        /* Wait for status of current process */
        int status;
        if(waitpid(current->pid, &status, WNOHANG) < 0) { perror("Error waiting for background process."); exit(1); }
        
        /* Check if process has terminated */
        if(WIFEXITED(status) || WIFSIGNALED(status)) {
          printf("Background process %u terminated\n", current->pid);
          remove_process(&processes, prev);
        }      

        /* Step iteration */
        prev = current;
        current = current->next;
      }

    }
  }

  /* Free command resources */
  free_command(&command);
  free_processlist(&processes);

  return 0;
}

int change_working_directory(struct command *cmd) {
  int error;

  char new_dir[PATH_MAX];

  /* If path begins at root, use bare path */
  if(cmd->tokens[1][0] == '/') {
    strncpy(new_dir, cmd->text+3, PATH_MAX);

  /* Otherwise prepend current directory */
  } else {
    int cwd_len;
    if(getcwd(new_dir, PATH_MAX) == NULL) { perror("Failed to get current working directory"); exit(1); }

    /* Get append start position */
    cwd_len = strlen(new_dir);
    if(cwd_len > 1) { cwd_len++; } 

    /* Append command path */
    new_dir[cwd_len-1] = '/';
    strncpy(new_dir + cwd_len, cmd->text+3, PATH_MAX - cwd_len);
  }

  /* Try changing directory */
  error = chdir(new_dir);
  if(error != 0) {
    perror("Failed to change directory");
    error = chdir(getenv("HOME"));
    if(error != 0) {
      perror("Failed to change directory");
      error = chdir("/");
    }
  }
  if(error != 0) { perror("Failed to change directory"); }

  return 0;
}

int execute_command(struct command *cmd, struct processlist *processes) {
  pid_t childpid;

  if((childpid = fork()) < 0) { perror("Error forking process"); return -1; }

  if(childpid == 0) {
    execvp(cmd->tokens[0], cmd->tokens);
    perror("Error executing child process");
    exit(1);
  } else {
    /* Ignore SIGINT in parent*/
    struct sigaction old;
    install_signal_handler(&old);

#ifdef STATISTICS
    struct timer process_timer;
    if(cmd->bg == 0) {
      printf("Spawned foreground process pid: %u\n", childpid);
      start_timer(&process_timer);
    } else {
      printf("Spawned background process pid: %u\n", childpid);
    }
#endif    

    /* If this is a foreground process, wait until it finishes completely */
    if(cmd->bg == 0) {
      int status;
      do {
        waitpid(childpid, &status, WUNTRACED | WCONTINUED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    /* Add the just started process to the list of background processes. */
    } else {
      add_process(processes, childpid);
    }

    /* Restore SIGINT handling on child termination */
    restore_signal_handler(&old);

#ifdef STATISTICS
    if(cmd->bg == 0) {
      printf("Foreground process %u terminated\n", childpid);
      int elapsed = end_timer(&process_timer);
      printf("Wallclock time: %lf ms.\n", ((double)elapsed)/1000);
    }
#endif 
  }

  return 0;

}

void install_signal_handler(struct sigaction *old) {
  struct sigaction action;
  action.sa_sigaction = &signal_handler;
  action.sa_mask = 0;
  action.sa_flags = SA_SIGINFO;

  printf("Installing\n");
  sigaction(SIGINT, &action, old);
  sigaction(SIGCHLD, &action, old);
}

void restore_signal_handler(struct sigaction *old) {
  printf("Restoring\n");
  sigaction(SIGINT, old, NULL);
  sigaction(SIGCHLD, old, NULL);
}

void signal_handler(int signal, siginfo_t *info, void *context) {
  printf("Signal: %d\n", signal);
}