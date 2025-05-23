
#include "cmap-mem.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-kernel.h"
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

  BLOCK_FREE * block_free_stree;
} INTERNAL;

static INTERNAL internal = {0, NULL, NULL, NULL};

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM mem = {};
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

static CMAP_STREE_NODE * node(void * node)
{
  return &((BLOCK_FREE *)node) -> node;
}

static int block_free_eval(void * node, void * data)
{
  int size = block_size((BLOCK *)node);
  return (size - *(int *)data);
}

static CMAP_STREE_RUNNER CMAP_STREE_RUNNER_NAME(block_free) =
{
  node,
  block_free_eval,
  NULL,
  CMAP_T, CMAP_F
};

/*******************************************************************************
*******************************************************************************/

static BLOCK_FREE * find_block_free(int alloc_size)
{
  return (BLOCK_FREE *)CMAP_STREE_FINDFN(block_free, internal.block_free_stree,
    &alloc_size);
}

static void free_block(BLOCK * block)
{
  block -> free = CMAP_T;

  int size = block_size(block);
  CMAP_STREE_ADDFN(block_free, &internal.block_free_stree, block, &size);
}

static void alloc_block(BLOCK_FREE * block)
{
  CMAP_STREE_RMFN(block_free, &internal.block_free_stree, block);

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
    cmap_log_public.fatal("Unable to allocate new memory.");
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
  cmap_consumedtime_public.start(&consumed_time);
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
  cmap_consumedtime_public.stop(&consumed_time);
#endif

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void free_(void * ptr)
{
  if(ptr == NULL) return;

#ifdef CONSUMED_TIME
  cmap_consumedtime_public.start(&consumed_time);
#endif

  BLOCK * block = (BLOCK *)(ptr - sizeof(BLOCK));
  if(!is_block(block)) cmap_log_public.fatal("Invalid block ???");
  if(block -> free) cmap_log_public.fatal("Memory address already free !!!");

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
  cmap_consumedtime_public.stop(&consumed_time);
#endif
}

/*******************************************************************************
*******************************************************************************/

static void delete()
{
}

static CMAP_MEM * instance(int chunk_size)
{
  if(mem_ptr == NULL)
  {
    internal.chunk_size = (chunk_size > CHUNK_SIZE_MIN) ?
      chunk_size : CMAP_KERNEL_INSTANCE -> cfg() -> mem.chunk_size;

    mem.delete = delete;
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

#ifdef CONSUMED_TIME
static void log_consumed_time(char lvl)
{
  cmap_consumedtime_public.log(lvl, &consumed_time, "memory");
}
#endif

/*******************************************************************************
*******************************************************************************/

const CMAP_MEM_PUBLIC cmap_mem_public =
{
  instance,
  state,
  is_this,
#ifdef CONSUMED_TIME
  log_consumed_time
#endif
};
