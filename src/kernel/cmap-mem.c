
#include "cmap-mem.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-config.h"
#include "cmap-log.h"
#include "cmap-stree.h"

#ifdef CONSUMED_TIME
#include "cmap-consumedtime.h"
#endif

/*******************************************************************************
*******************************************************************************/

#define VAL_1 0x55
#define VAL_2 0x2a

#define CHUNK_SIZE_MIN (sizeof(BLOCK_FREE) + sizeof(BLOCK))

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

  CMAP_STREE_NODE node;
} BLOCK_FREE;

#define BLOCK_FROM_NODE(node) (((BLOCK *)node) - 1)
#define BLOCK_FREE_FROM_NODE(node) ((BLOCK_FREE *)BLOCK_FROM_NODE(node))

#define NODE_FROM_BLOCK(block) ((CMAP_STREE_NODE *)(block + 1))
#define NODE_FROM_BLOCK_FREE(block_free) NODE_FROM_BLOCK(((BLOCK *)block_free))

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

  CMAP_STREE_NODE * block_free_stree;
} INTERNAL;

static INTERNAL internal = {0, NULL, NULL, NULL};

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM mem;
static CMAP_MEM * mem_ptr = NULL;

#ifdef CONSUMED_TIME
static CMAP_CONSUMEDTIME_US consumed_time = {0};
#endif

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

static int64_t block_free_eval(CMAP_STREE_NODE * node, void * data)
{
  int size = block_size(BLOCK_FROM_NODE(node));
  return (size - *(int *)data);
}

static CMAP_STREE_RUNNER CMAP_STREE_RUNNER_NAME(block_free) =
  {block_free_eval, NULL, CMAP_T, CMAP_F};

/*******************************************************************************
*******************************************************************************/

static BLOCK_FREE * find_block_free(int alloc_size)
{
  CMAP_STREE_NODE * node = CMAP_STREE_FINDFN(block_free,
    internal.block_free_stree, &alloc_size);
  return (node == NULL) ? NULL : BLOCK_FREE_FROM_NODE(node);
}

static void free_block(BLOCK * block)
{
  block -> free = CMAP_T;

  int size = block_size(block);
  CMAP_STREE_ADDFN(block_free, &internal.block_free_stree,
    NODE_FROM_BLOCK(block), &size);
}

static void alloc_block(BLOCK_FREE * block)
{
  CMAP_STREE_RMFN(&internal.block_free_stree, NODE_FROM_BLOCK_FREE(block));

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
    cmap_log_fatal("Unable to allocate new memory.");
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
#ifdef CONSUMED_TIME
  cmap_consumedtime_start(&consumed_time);
#endif

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

#ifdef CONSUMED_TIME
  cmap_consumedtime_stop(&consumed_time);
#endif

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void free_(void * ptr)
{
  if(ptr == NULL) return;

#ifdef CONSUMED_TIME
  cmap_consumedtime_start(&consumed_time);
#endif

  BLOCK * block = (BLOCK *)(ptr - sizeof(BLOCK));
  if(!is_block(block)) cmap_log_fatal("Invalid block ???");
  if(block -> free) cmap_log_fatal("Memory address already free !!!");

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

#ifdef CONSUMED_TIME
  cmap_consumedtime_stop(&consumed_time);
#endif
}

/*******************************************************************************
*******************************************************************************/

static void delete()
{
  /* TODO */
}

static CMAP_MEM * instance()
{
  if(mem_ptr == NULL)
  {
    mem_ptr = cmap_config_mem();
    if(mem_ptr == NULL)
    {
      internal.chunk_size = cmap_config_mem_chunk_size();
      if(internal.chunk_size < CHUNK_SIZE_MIN)
        internal.chunk_size = CMAP_MEM_CHUNK_SIZE_DFT;

      mem.delete = delete;
      mem.alloc = alloc;
      mem.free = free_;

      mem_ptr = &mem;
    }
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

CMAP_MEM_STATE * cmap_mem_state()
{
  static CMAP_MEM_STATE state;
  upd_state(&state);
  return &state;
}

/*******************************************************************************
*******************************************************************************/

void cmap_mem_delete()
{
  instance() -> delete();
}

/*******************************************************************************
*******************************************************************************/

void * cmap_mem_alloc(int size)
{
  return instance() -> alloc(size);
}

void cmap_mem_free(void * ptr)
{
  instance() -> free(ptr);
}

/*******************************************************************************
*******************************************************************************/

char cmap_mem_is_internal()
{
  return (instance() == &mem);
}

/*******************************************************************************
*******************************************************************************/

#ifdef CONSUMED_TIME
void cmap_mem_log_consumed_time(char lvl)
{
  cmap_consumedtime_log(lvl, &consumed_time, "memory");
}
#endif
