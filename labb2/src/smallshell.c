#include <sys/types.h> /* definierar typen pid_t */
#include <sys/wait.h> /* definerar wait() */
#include <stdio.h> /* definierar bland annat stderr */
#include <stdlib.h> /* definierar bland annat rand() och RAND_MAX */
#include <unistd.h> /* definierar bland annat pipe() */
#include <string.h>
#include <limits.h>

#include "../include/command.h"
#include "../include/timer.h"

#define STATISTICS

int change_working_directory(struct command *cmd);

int main(
  int     argc, /* The number of command line arguments */
  char  **argv, /* The array of command line arguments  */
  char  **envp) /* The array of envirnment variables    */
{
  /* Allocate command string variables */
  struct command command;
  init_command(&command);

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
        execute_command(&command);
      }
    }
  }

  /* Free command resources */
  free_command(&command);

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

  printf("changing to: %s\n", new_dir);

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

