#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "brk.h"

/**
 * Constants
 */
/* Used to check that a block was allocated with malloc */
#define BLOCK_CANARY 0xD15EA5E 
/* Used to calculate page sizes */
#define PAGE_SIZE sysconf(_SC_PAGESIZE)

/* Names of strategies */
#define STRATEGY_FIRSTFIT 1
#define STRATEGY_BESTFIT 2
#define STRATEGY_WORSTFIT 3 /* Not supported */
#define STRATEGY_QUICKFIT 4 /* Not supported */

/* Make sure strategy is set to a valid value */
#ifndef STRATEGY
  #define STRATEGY STRATEGY_FIRSTFIT
#endif

#if (STRATEGY != 1) && (STRATEGY != 2)
  #error STRATEGY must be STRATEGY_FIRSTFIT or STRATEGY_BESTFIT
#endif

/* Compatibility for strange systems */
#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
  # define MAP_ANONYMOUS MAP_ANON
#endif

/**
 * Declarations
 */

 typedef long Align;

/* Declare block header as linked list node */
union block {
  /* Actual block data */
  struct {
    union block *next;
    unsigned int size;
    unsigned long canary;
  } header;

  /* Union part to make header 2^n bits to simplify size arithmetics */
  struct {
    Align align1;
    Align align2;
    Align align3;
    Align align4;
  } align;
};
typedef union block Block;

static Block *mapunits(unsigned int nunits);
static void init_header(Block *header, size_t size);

/* Start of list */
static Block firstblock;
static Block *freelist = NULL;


/**
 * Implementations
 */
#ifdef MMAP

static void * __endHeap = 0;

void * endHeap(void)
{
  if(__endHeap == 0) __endHeap = sbrk(0);
  return __endHeap;
}
#endif

/* Init block header with size and canary value */
static void init_header(Block *header, size_t size) {
  header->header.size = size;
  header->header.canary = BLOCK_CANARY;
}

static Block * mapunits(unsigned int nunits) {
  unsigned int npages;
  void *blockmem;
  Block *block;

#ifdef MMAP
  /* Make sure __endHeap is initialized */
  endHeap();
#endif

  /* memory = PAGE_SIZE * (ceil(nunits/PAGE_SIZE) + 1) */
  npages = ((nunits*sizeof(Block))-1)/PAGE_SIZE + 1;
  blockmem = mmap(NULL, npages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  nunits = (npages * PAGE_SIZE)/sizeof(Block);
  __endHeap += npages * PAGE_SIZE;

  /* Check that we got the memory */
  if(blockmem == (void *) -1) {
    perror("Failed to get more memory");
    return NULL;
  }

  /* Init block header */
  block = (Block *) blockmem;
  init_header(block, nunits);

  /* Put block in freelist */
  free((void *)(block+1));

  /* Return freelist (which contains new block) */
  return freelist;
}

void *malloc(size_t size) {
  unsigned int nunits;
  Block *prev, *current, *candidate;

  /* Validate size */
  if(size == 0) {
    return NULL;
  }

  /* Init list first time */
  if(freelist == NULL) {
    init_header(&firstblock, 0);
    firstblock.header.next = &firstblock;
    freelist = &firstblock;
  }

  /* Compute actual required memory, ceil(size/headersize)+1 */
  nunits = (size + sizeof(Block) - 1)/sizeof(Block) + 1;

  candidate = &firstblock;

  current = freelist;
  /* Look in the freelist for an appropriate block */
  for(prev = current, current = current->header.next; ;
      prev = current, current = current->header.next) {

    /* Found big enough block */
    if(current->header.size >= nunits) {

      /* If exact match, use it no matter strategy */
      if(current->header.size == nunits) {
        candidate = current;
        break;
      
      /* Otherwise, pick appropriate according to strategy */
      } else {
#if STRATEGY == STRATEGY_FIRSTFIT
        /* Use this as the candidate */
        candidate = current;
        break;

#elif STRATEGY == STRATEGY_BESTFIT
        /* If less waste, pick as new candidate */
        if(candidate->header.size == 0 || candidate->header.size > current->header.size) {
          candidate = current;
          freelist = prev;
        }

#endif
      }
    }

    /* Wrapped around, allocate more blocks */
    if(current == freelist) {

#if STRATEGY == STRATEGY_BESTFIT
      /* Already have candidate */
      if(candidate->header.size > 0) {
        break;
      }
#endif

      /* Allocate more blocks */
      current = mapunits(nunits);
      if(current == NULL) {
        return NULL;
      }
    }
  }

  /* Set start of freelist at prev */
  freelist = prev;

  /* Split block appriopriately and return tail end */
  if(candidate->header.size == nunits) {
    prev->header.next = current->header.next;
  } else {
    candidate->header.size -= nunits;
    candidate += candidate->header.size;
    init_header(candidate, nunits);
  }
  
  /* Return pointer to where memory actually starts */
  return (void *)(candidate+1);
}

void free(void *ptr) {
  Block *block, *current;

  /* Validate pointer */
  if(ptr == NULL || ptr < endHeap()) {
    return;
  }

  /* Get block header */
  block = (Block *)ptr - 1;

  /* Validate header */
  if(block->header.canary != BLOCK_CANARY) {
    errno = EPERM;
    return;
  }

  /**
   * Return to free list
   */
  /* Find location between two blocks */
  for(current = freelist; !(block > current && block < current->header.next); current = current->header.next) {
    /* Are we about to wrap round? */
    if(current >= current->header.next && (block > current || block < current->header.next)) {
      break; /* End of list */
    }
  }

  /* If next to, merge with following block */
  if(block + block->header.size == current->header.next) {
    block->header.size += current->header.next->header.size;
    block->header.next = current->header.next->header.next;
  }
  /* Else, just add to list */
  else {
    block->header.next = current->header.next;
  }

  /* If next to, Merge with previous block */
  if(current + current->header.size == block) {
    current->header.size += block->header.size;
    current->header.next = block->header.next;
  /* Else, just add to list */
  } else {
    current->header.next = block;
  }

  /* Make free list start before newly freed block */
  freelist = current;
}

void *realloc(void *ptr, size_t size) {
  unsigned int new_size, old_size, copy_size;
  void *new_mem;
  Block *header;

  /* No previous memory, simply use malloc */
  if(ptr == NULL) {
    return malloc(size);
  }

  /* New size is 0, free memory */
  if(size == 0) {
    free(ptr);
    return NULL;
  }

  /* Validate header */
  header = (Block *)ptr - 1;
  if(header->header.canary != BLOCK_CANARY) {
    errno = EPERM;
    return NULL;
  }

  /* Allocate new memory */
  new_mem = malloc(size);
  if(new_mem == NULL) {
    return NULL;
  }
 
  /* copy_size = Min(new_size, old_size) */
  new_size = ((Block *)new_mem - 1)->header.size - 1;
  old_size = header->header.size - 1;
  copy_size = new_size < old_size ? new_size : old_size;

  /* copy old memory and free old memory */
  memcpy(new_mem, ptr, copy_size*sizeof(Block));
  free(ptr);

  return new_mem;
}