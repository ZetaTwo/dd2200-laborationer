#ifndef __COMMAND__
#define __COMMAND__

#include <stdlib.h>

#define STRING_LENGTH 80
#define TOKENS_LENGTH 1

struct command {
  char *text;
  int max_len;
  int len;

  char *token_text;
  char **tokens;
  int max_tokens;
  int len_tokens;
  int bg;
};

int init_command(struct command *cmd);
int free_command(struct command *cmd);

int read_command(struct command *cmd);
int parse_command(struct command *cmd);

int execute_command(struct command *cmd);

#endif