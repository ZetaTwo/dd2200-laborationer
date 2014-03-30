#include "../include/command.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int init_command(struct command *cmd) {
  cmd->max_len = STRING_LENGTH;
  cmd->text = (char *)malloc(sizeof(char)*cmd->max_len);
  cmd->len = 0;

  cmd->max_tokens = TOKENS_LENGTH;
  cmd->len_tokens = 0;
  cmd->tokens = (char **)malloc(sizeof(char *)*cmd->max_tokens);
  cmd->token_text = 0;

  return 0;
}

int free_command(struct command *cmd) {
  free(cmd->text);
  free(cmd->token_text);
  free(cmd->tokens);

  return 0;
}

int read_command(struct command *cmd) {
  cmd->len = 0;
  int c;
  int prefix = 1;
  while(( c = getchar() ) != '\n' && c != EOF) {
    /* Skip leading spaces */
    if(prefix && c == ' ') {
      continue;
    } else {
      prefix = 0;
    }

    cmd->text[cmd->len++] = c;

    /*
     * Double string length on max. Note: ammortized O(1)
     *
     */
    if(cmd->len >= cmd->max_len) {
      cmd->max_len *= 2;
      cmd->text = (char *)realloc(cmd->text, sizeof(char)*cmd->max_len);
    }
  }
  cmd->text[cmd->len++] = 0;

  return cmd->len;
}

int parse_command(struct command *cmd) {
  /* Reset tokens length */
  cmd->len_tokens = 0;
  cmd->bg = 0;
  
  /* Create parseable copy of text */
  if(cmd->token_text != 0) { free(cmd->token_text); }
  cmd->token_text = (char *)malloc(sizeof(char)*cmd->len);
  strncpy(cmd->token_text, cmd->text, cmd->len);
  
  /* Parse string into tokens */
  char *parse = cmd->token_text;
  char *token;
  while((token = strtok(parse, " ")) != NULL) {
    parse = NULL;

    /*
     * If token is '&', skip the rest and mark as background command
     *
     */
    if(strcmp("&", token) == 0) {
      cmd->bg = 13;
      break;
    }

    cmd->tokens[cmd->len_tokens++] = token;

    /*
     * Double command vector length on max. Note: ammortized O(1)
     *
     */
    if(cmd->len_tokens >= cmd->max_tokens) {
      cmd->max_tokens *= 2;
      cmd->tokens = (char **)realloc(cmd->tokens, sizeof(char *)*cmd->max_tokens);
    }
  }
  cmd->tokens[cmd->len_tokens++] = NULL;

  return cmd->len_tokens;
}

int execute_command(struct command *cmd) {
  int childpid;
  if((childpid = fork()) < 0) { perror("Error forking process"); return -1; }

  if(childpid == 0) {
    execvp(cmd->tokens[0], cmd->tokens);
    perror("Error executing child process");
    exit(1);
  } else {

#ifdef STATISTICS
    printf("Spawned foreground process pid: %u\n", childpid);
    struct timer process_timer;
    start_timer(&process_timer);
#endif    

    wait(NULL);

#ifdef STATISTICS
    printf("Foreground process %u terminated\n", childpid);
    int elapsed = end_timer(&process_timer);
    printf("Wallclock time: %lf ms.\n", ((double)elapsed)/1000);
#endif 
  }

  return 0;

}