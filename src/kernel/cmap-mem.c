
#include "cmap-mem.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-log.h"
#include "cmap-tree.h"

/*******************************************************************************
*******************************************************************************/

#define VAL_1 0x55
#define VAL_2 0x2a

#define CHUNK_SIZE_MIN (sizeof(BLOCK_FREE) + sizeof(BLOCK))
#define CHUNK_SIZE_DFT (1 << 20)

#define BLOCK_SIZE_MIN (sizeof(BLOCK_FREE) - sizeof(BLOCK))

/*******************************************************************************
*******************************************************************************/

typedef struct BLOCK_s BLOCK;

struct BLOCK_s
{
  char valid1, valid2, free;

  BLOCK * prev, * next;
};

typedef struct
{
  BLOCK super;

  CMAP_TREE_NODE node;
} BLOCK_FREE;

/*******************************************************************************
*******************************************************************************/

typedef struct CHUNK_s CHUNK;

struct CHUNK_s
{
  int size;

  CHUNK * prev, * next;
};

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int chunk_size;

  CHUNK * chunk_list, * chunk_tail_list;

  BLOCK_FREE * block_free_tree;
} INTERNAL;

static INTERNAL internal = {CHUNK_SIZE_DFT, NULL, NULL, NULL};

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM mem = {};
static CMAP_MEM * mem_ptr = NULL;

/*******************************************************************************
*******************************************************************************/

static void error(const char * msg)
{
  cmap_log_public.fatal(msg);
  CMAP_KERNEL_INSTANCE -> fatal();
}

/*******************************************************************************
*******************************************************************************/

static inline void valid_block(BLOCK * block)
{
  block -> valid1 = VAL_1;
  block -> valid2 = VAL_2;
}

static inline void invalid_block(BLOCK * block)
{
  block -> valid1 = 0;
  block -> valid2 = 0;
}

static int is_block(BLOCK * block)
{
  return ((block -> valid1 == VAL_1) && (block -> valid2 == VAL_2));
}

static inline int block_size(BLOCK * block)
{
  if(block -> next == NULL) return 0;
  else
  {
    int size = (void *)block -> next - (void *)block;
    return (size - sizeof(BLOCK));
  }
}

static void init_block(BLOCK * block)
{
  valid_block(block);

  block -> free = CMAP_F;
  block -> prev = NULL;
  block -> next = NULL;
}

static void add_block(BLOCK * block, BLOCK * prev)
{
  block -> prev = prev;

  block -> next = prev -> next;
  prev -> next = block;

  if(block -> next != NULL) block -> next -> prev = block;
}

static void rm_block(BLOCK * block, BLOCK * prev)
{
  invalid_block(block);

  prev -> next = block -> next;

  if(prev -> next != NULL) prev -> next -> prev = prev;
}

/*******************************************************************************
*******************************************************************************/

#define WAY_FN(way) \
static void ** way(CMAP_TREE_RUNNER * this, void * node) \
{ \
  return &((BLOCK_FREE *)node) -> node.way; \
}

CMAP_TREE_LOOP(WAY_FN)

static int CMAP_TREE_EVALFN_NAME(block_free)(CMAP_TREE_RUNNER * this,
  void * node, void * data)
{
  int size = block_size((BLOCK *)node);
  return (size - *(int *)data);
}

#define WAY_SET(way) way,

static CMAP_TREE_RUNNER CMAP_TREE_RUNNER_NAME(block_free) =
{
  NULL,
  CMAP_TREE_LOOP(WAY_SET)
  CMAP_TREE_EVALFN_NAME(block_free),
  cmap_tree_usable_false,
  cmap_tree_usable_true
};

/*******************************************************************************
*******************************************************************************/

static BLOCK_FREE * find_block_free(int alloc_size)
{
  return (BLOCK_FREE *)CMAP_TREE_FINDFN(block_free, internal.block_free_tree,
    &alloc_size);
}

static void free_block(BLOCK * block)
{
  block -> free = CMAP_T;

  int size = block_size(block);
  CMAP_TREE_ADDFN(block_free, &internal.block_free_tree, block, &size);
}

static void alloc_block(BLOCK_FREE * block)
{
  CMAP_TREE_RMFN(block_free, &internal.block_free_tree, block);

  ((BLOCK *)block) -> free = CMAP_F;
}

/*******************************************************************************
*******************************************************************************/

static inline BLOCK * chunk_block_list(CHUNK * chunk)
{
  return (BLOCK *)(chunk + 1);
}

static inline BLOCK * chunk_block_tail_list(CHUNK * chunk)
{
  BLOCK * block = (BLOCK *)(((void *)(chunk + 1)) + chunk -> size);
  return (block - 1);
}

static void init_chunk(CHUNK * chunk, int size)
{
  chunk -> size = size;
  chunk -> prev = NULL;
  chunk -> next = NULL;

  BLOCK * block_list = chunk_block_list(chunk),
        * block_tail_list = chunk_block_tail_list(chunk);
  init_block(block_list);
  init_block(block_tail_list);

  add_block(block_tail_list, block_list);
  free_block(block_list);
}

static CHUNK * create_chunk(int alloc_size)
{
  int chunk_size = internal.chunk_size;
  alloc_size += 2 * sizeof(BLOCK);
  if(alloc_size > chunk_size) chunk_size = alloc_size;

  CHUNK * chunk = (CHUNK *)malloc(sizeof(CHUNK) + chunk_size);
  if(chunk == NULL)
  {
    error("Unable to allocate new memory.");
    return NULL;
  }
  else
  {
    init_chunk(chunk, chunk_size);

    if(internal.chunk_tail_list != NULL)
    {
      internal.chunk_tail_list -> next = chunk;
      chunk -> prev = internal.chunk_tail_list;
    }
    else
    {
      internal.chunk_list = chunk;
    }
    internal.chunk_tail_list = chunk;

    return chunk;
  }
}

/*******************************************************************************
*******************************************************************************/

static BLOCK_FREE * create_block_free(int alloc_size)
{
  CHUNK * chunk = create_chunk(alloc_size);
  if(chunk == NULL) return NULL;

  return (BLOCK_FREE *)chunk_block_list(chunk);
}

/*******************************************************************************
*******************************************************************************/

static void * alloc(int size)
{
  BLOCK_FREE * block = find_block_free(size);
  if(block == NULL) block = create_block_free(size);
  if(block == NULL) return NULL;

  void * ret = ((void *)block) + sizeof(BLOCK);

  alloc_block(block);

  int size_real = size;
  if(size_real < BLOCK_SIZE_MIN) size_real = BLOCK_SIZE_MIN;
  int size_free = block_size((BLOCK *)block) - size_real;
  if(size_free >= sizeof(BLOCK_FREE))
  {
    BLOCK * new_block = (BLOCK *)(ret + size_real);
    valid_block(new_block);
    add_block(new_block, (BLOCK *)block);
    free_block(new_block);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void free_(void * ptr)
{
  if(ptr == NULL) return;

  BLOCK * block = (BLOCK *)(ptr - sizeof(BLOCK));
  if(!is_block(block)) error("Invalid block ???");
  if(block -> free) error("Memory address already free !!!");

  BLOCK * next = block -> next, * prev = block -> prev;

  if(next -> free)
  {
    alloc_block((BLOCK_FREE *)next);
    rm_block(next, block);
  }

  if((prev == NULL) || !prev -> free)
  {
    free_block(block);
  }
  else
  {
    rm_block(block, prev);
    alloc_block((BLOCK_FREE *)prev);
    free_block(prev);
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM * init(int chunk_size)
{
  if(mem_ptr == NULL)
  {
    internal.chunk_size =
      (chunk_size > CHUNK_SIZE_MIN) ? chunk_size : CHUNK_SIZE_DFT;

    mem.alloc = alloc;
    mem.free = free_;

    mem_ptr = &mem;
  }
  return mem_ptr;
}

/*******************************************************************************
*******************************************************************************/

static void upd_state_chunk(CMAP_MEM_STATE * state, CHUNK * chunk)
{
  BLOCK * block = chunk_block_list(chunk);
  while(block != NULL)
  {
    state -> nb_block++;
    if(block -> free)
    {
      state -> nb_block_free++;
      state -> size_free += block_size(block);
    }
    else
    {
      state -> size_alloc += block_size(block);
    }

    block = block -> next;
  }
}

static void upd_state(CMAP_MEM_STATE * state)
{
  state -> nb_chunk = 0;
  state -> nb_block = 0;
  state -> nb_block_free = 0;
  state -> size_alloc = 0;
  state -> size_free = 0;

  CHUNK * chunk = internal.chunk_list;
  while(chunk != NULL)
  {
    state -> nb_chunk++;
    upd_state_chunk(state, chunk);

    chunk = chunk -> next;
  }
}

static CMAP_MEM_STATE * state()
{
  static CMAP_MEM_STATE state;
  upd_state(&state);
  return &state;
}

/*******************************************************************************
*******************************************************************************/

static char is_this(CMAP_MEM * mem_)
{
  return (mem_ == &mem);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_MEM_PUBLIC cmap_mem_public =
{
  init,
  state,
  is_this
};
