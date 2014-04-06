/* command
 *
 * This module implements the command structure
 * which stores information about the inputed
 * command and functions for inputing and parsing it
 */

#ifndef __COMMAND__
#define __COMMAND__

#include <stdlib.h>

#define STRING_LENGTH 80
#define TOKENS_LENGTH 4

/* struct command 
 *
 * A structure containing the information
 * about a command, including the raw text,
 * the tokenized command and whether it is
 * a background or foreground command
 */
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

/** init_command
 *
 * Sets up the command structure for usage.
 */
int init_command(struct command *cmd);

/** free_command
 *
 * Frees up the resources used by the command.
 */
int free_command(struct command *cmd);

/** read_command
 *
 * Reads a new command from the standard input.
 */
int read_command(struct command *cmd);

/** parse_command
 *
 * Parses the command read by read_command.
 */
int parse_command(struct command *cmd);


#endif