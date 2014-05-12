#include <stdlib.h>
#include <stdio.h>

#define NUM_BLOCKS 2048
#define MAX_ALLOC 2048

void *blocks[NUM_BLOCKS];

int main(int argc, char **argv) {
  /* Set up benchmark */
  unsigned long iterations, rechance, i;
  int action, target, size;

  if(argc != 3) {
    printf("Usage: %s iterations realloc\n", argv[0]);
    return 1;
  } else {
    iterations = atoi(argv[1]);
    rechance = atoi(argv[2]);
  }

  for(i = 0; i < NUM_BLOCKS; i++) {
    blocks[i] = NULL;
  }

  /* Start of benchmark */
  for(i = 0; i < iterations; i++) {
    /* Random action and target */
    action = rand() % 10;
    target = rand() % NUM_BLOCKS;
    size = rand() % MAX_ALLOC;

    /* Realloc */
    if(action < rechance) {
      blocks[target] = realloc(blocks[target], size);

    /* Malloc/Free */
    } else {
      /* Malloc */
      if(blocks[target] == NULL) {
        blocks[target] = malloc(size);

      /* Free */
      } else {
        free(blocks[target]);
        blocks[target] = NULL;
      }
    }
  }

  return 0;
}