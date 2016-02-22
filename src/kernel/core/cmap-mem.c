
#include "cmap-mem.h"

#include <stdlib.h>
#include <stdio.h>
#include "cmap-common.h"
#include "cmap-kernel.h"
#include "cmap-tree.h"

/*******************************************************************************
*******************************************************************************/

#define VAL_1 0x55
#define VAL_2 0x2a

#define CHUNK_SIZE_MIN (sizeof(BLOCK) + sizeof(BLOCK_FREE))
#define CHUNK_SIZE_DFT (1 << 20)

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM mem_;
static CMAP_MEM * mem_ptr_ = NULL;

/*******************************************************************************
*******************************************************************************/

typedef struct BLOCK_s BLOCK;

struct BLOCK_s
{
  char valid1_, valid2_, free_;

  BLOCK * prev_, * next_;
};

typedef struct BLOCK_FREE_s BLOCK_FREE;

struct BLOCK_FREE_s
{
  BLOCK super;

  CMAP_TREE_STRUCT;
};

/*******************************************************************************
*******************************************************************************/

typedef struct CHUNK_s CHUNK;

struct CHUNK_s
{
  int size_;

  CHUNK * prev_, * next_;
};

/*******************************************************************************
*******************************************************************************/

static int chunk_size_ = CHUNK_SIZE_DFT;

static CHUNK * chunk_list_ = NULL, * chunk_tail_list_ = NULL;

static BLOCK_FREE * block_free_tree_ = NULL;

/*******************************************************************************
*******************************************************************************/

static void error(const char * msg)
{
  fprintf(stderr, "[ERROR]  %s\n", msg);
  cmap_kernel() -> fatal();
}

/*******************************************************************************
*******************************************************************************/

static void valid_block(BLOCK * block)
{
  block -> valid1_ = VAL_1;
  block -> valid2_ = VAL_2;
}

static int is_block(BLOCK * block)
{
  return ((block -> valid1_ == VAL_1) && (block -> valid2_ == VAL_2));
}

static inline block_size(BLOCK * block)
{
  if(block -> next_ == NULL) return 0;
  else
  {
    int size = (void *)block -> next_ - (void *)block;
    return (size - sizeof(BLOCK));
  }
}

static void init_block(BLOCK * block)
{
  valid_block(block);

  block -> free_ = CMAP_F;
  block -> prev_ = NULL;
  block -> next_ = NULL;
}

static void add_block(BLOCK * block, BLOCK * prev)
{
  block -> prev_ = prev;

  block -> next_ = prev -> next_;
  prev -> next_ = block;

  if(block -> next_ != NULL) block -> next_ -> prev_ = block;
}

static void rm_block(BLOCK * block, BLOCK * prev)
{
  prev -> next_ = block -> next_;

  if(prev -> next_ != NULL) prev -> next_ -> prev_ = prev;
}

/*******************************************************************************
*******************************************************************************/

int CMAP_TREE_EVALFN_NAME(block_free)(CMAP_TREE_RUNNER * this, void * node)
{
  int size = block_size((BLOCK *)node);
  return (size - *(int *)(this -> internal_));
}

CMAP_TREE_RUNNER(BLOCK_FREE, block_free, false, true)

/*******************************************************************************
*******************************************************************************/

static BLOCK_FREE * find_block_free(int alloc_size)
{
  CMAP_TREE_RUNNER_INIT(block_free, &alloc_size)
  return (BLOCK_FREE *)CMAP_TREE_FINDFN(block_free, block_free_tree_);
}

static void free_block(BLOCK * block)
{
  block -> free_ = CMAP_T;

  int size = block_size(block);
  CMAP_TREE_RUNNER_INIT(block_free, &size)
  CMAP_TREE_ADDFN(block_free, &block_free_tree_, block);
}

static void alloc_block(BLOCK_FREE * block)
{
  CMAP_TREE_RUNNER_INIT(block_free, NULL)
  CMAP_TREE_RMFN(block_free, &block_free_tree_, block);

  ((BLOCK *)block) -> free_ = CMAP_F;
}

/*******************************************************************************
*******************************************************************************/

static inline BLOCK * chunk_block_list(CHUNK * chunk)
{
  return (BLOCK *)(chunk + 1);
}

static inline BLOCK * chunk_block_tail_list(CHUNK * chunk)
{
  BLOCK * block = (BLOCK *)(((void *)(chunk + 1)) + chunk -> size_);
  return (block - 1);
}

static void init_chunk(CHUNK * chunk, int size)
{
  chunk -> size_ = size;
  chunk -> prev_ = NULL;
  chunk -> next_ = NULL;

  BLOCK * block_list = chunk_block_list(chunk),
        * block_tail_list = chunk_block_tail_list(chunk);
  init_block(block_list);
  init_block(block_tail_list);

  add_block(block_tail_list, block_list);
  free_block(block_list);
}

static CHUNK * create_chunk(int alloc_size)
{
  int chunk_size = chunk_size_;
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

    if(chunk_tail_list_ != NULL)
    {
      chunk_tail_list_ -> next_ = chunk;
      chunk -> prev_ = chunk_tail_list_;
    }
    else
    {
      chunk_list_ = chunk;
    }
    chunk_tail_list_ = chunk;

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

static void * _alloc(int size)
{
  BLOCK_FREE * block = find_block_free(size);
  if(block == NULL) block = create_block_free(size);
  if(block == NULL) return NULL;

  void * ret = ((void *)block) + sizeof(BLOCK);

  alloc_block(block);

  int size_waste = block_size((BLOCK *)block) - size;
  if(size_waste >= sizeof(BLOCK_FREE))
  {
    BLOCK * new_block = (BLOCK *)(ret + size);
    valid_block(new_block);
    add_block(new_block, (BLOCK *)block);
    free_block(new_block);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void _free(void * ptr)
{
  BLOCK * block = (BLOCK *)(ptr - sizeof(BLOCK));
  if(!is_block(block)) error("Invalid block ???");

  BLOCK * next = block -> next_, * prev = block -> prev_;

  if(next -> free_)
  {
    alloc_block((BLOCK_FREE *)next);
    rm_block(next, block);
  }

  if((prev == NULL) || !prev -> free_)
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

CMAP_MEM * cmap_mem_create(int chunk_size)
{
  if(mem_ptr_ == NULL)
  {
    chunk_size_ = (chunk_size > CHUNK_SIZE_MIN) ? chunk_size : CHUNK_SIZE_DFT;

    mem_.alloc = _alloc;
    mem_.free = _free;

    mem_ptr_ = &mem_;
  }
  return mem_ptr_;
}

/*******************************************************************************
*******************************************************************************/

static void upd_state_chunk(CMAP_MEM_STATE * state, CHUNK * chunk)
{
  BLOCK * block = chunk_block_list(chunk);
  while(block != NULL)
  {
    state -> nb_block_++;
    if(block -> free_)
    {
      state -> nb_block_free_++;
      state -> size_free_ += block_size(block);
    }
    else
    {
      state -> size_alloc_ += block_size(block);
    }

    block = block -> next_;
  }
}

static void upd_state(CMAP_MEM_STATE * state)
{
  state -> nb_chunk_ = 0;
  state -> nb_block_ = 0;
  state -> nb_block_free_ = 0;
  state -> size_alloc_ = 0;
  state -> size_free_ = 0;

  CHUNK * chunk = chunk_list_;
  while(chunk != NULL)
  {
    state -> nb_chunk_++;
    upd_state_chunk(state, chunk);

    chunk = chunk -> next_;
  }
}

CMAP_MEM_STATE * cmap_mem_state()
{
  static CMAP_MEM_STATE state;
  upd_state(&state);
  return &state;
}
