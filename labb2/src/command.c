/* command
 *
 * This module implements the command structure
 * which stores information about the inputed
 * command and functions for inputing and parsing it
 */

#include "../include/command.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/** init_command
 *
 * Sets up the command structure for usage.
 */
int init_command(
  struct command *cmd) { /* IN: The command structure to prepare for usage */

  /* Allocate an initial chunk of memory for the text. */
  cmd->max_len = STRING_LENGTH;
  cmd->text = (char *)malloc(sizeof(char)*cmd->max_len);
  cmd->len = 0;

  /* Allocate an initial chunk of memory for the tokens. */
  cmd->max_tokens = TOKENS_LENGTH;
  cmd->len_tokens = 0;
  cmd->tokens = (char **)malloc(sizeof(char *)*cmd->max_tokens);
  cmd->token_text = 0;

  return 0;
}

/** free_command
 *
 * Frees up the resources used by the command.
 */
int free_command(
  struct command *cmd) { /* IN: The command structure to free up. */

  /* Free the texts and array */
  free(cmd->text);
  free(cmd->token_text);
  free(cmd->tokens);

  return 0;
}

/** read_command
 *
 * Reads a new command from the standard input
 * and saves it in cmd.
 */
int read_command(
  struct command *cmd) { /* IN: The command structure to save the command in. */

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

/** parse_command
 *
 * Parses the command read by read_command.
 */
int parse_command(
  struct command *cmd) { /* IN: The command to parse. */

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
      cmd->bg = 1;
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