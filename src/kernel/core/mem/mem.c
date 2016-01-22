
#include "mem.h"

#include <stdlib.h>
#include <stdio.h>
#include "common.h"

/*******************************************************************************
*******************************************************************************/

#define VAL_1 0x55
#define VAL_2 0x2a

#define CHUNK_SIZE_MIN (sizeof(BLOCK) + sizeof(BLOCK_FREE))

/*******************************************************************************
*******************************************************************************/

typedef struct BLOCK_s BLOCK;

struct BLOCK_s
{
  char valid1_, valid2_, waste_, free_;

  BLOCK * prev_, * next_;
};

typedef struct BLOCK_FREE_s BLOCK_FREE;

struct BLOCK_FREE_s
{
  BLOCK super;

  BLOCK_FREE * ge_, * lt_, * parent_;
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

static CMAP_MEM mem_;

typedef struct
{
  int chunk_size_;

  CHUNK * chunk_list_, * chunk_tail_list_;

  BLOCK_FREE * block_free_tree_;
} INTERNAL;

static INTERNAL internal_;

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM_FACTORY mem_factory_;

/*******************************************************************************
*******************************************************************************/

static void error(const char * msg)
{
  printf("ERROR : %s\n", msg);
  exit(EXIT_FAILURE);
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

  block -> waste_ = 0;
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

static BLOCK_FREE * find_block_free(INTERNAL * internal, int alloc_size)
{
  BLOCK_FREE * cur = internal -> block_free_tree_, * result = NULL;
  while(cur != NULL)
  {
    int size_cur = block_size((BLOCK *)cur);
    if(size_cur == alloc_size)
    {
      result = cur;
      cur = NULL;
    }
    else if(size_cur > alloc_size)
    {
      result = cur;
      cur = cur -> lt_;
    }
    else
    {
      cur = cur -> ge_;
    }
  }
  return result;
}

static void free_block(INTERNAL * internal, BLOCK * block)
{
  block -> free_ = CMAP_T;

  BLOCK_FREE * block_free = (BLOCK_FREE *)block;
  block_free -> ge_ = NULL;
  block_free -> lt_ = NULL;

  BLOCK_FREE ** cur_ptr = &internal -> block_free_tree_, * parent = NULL;
  int size = block_size(block);

  while(*cur_ptr != NULL)
  {
    parent = *cur_ptr;

    int size_cur = block_size((BLOCK *)parent);
    if(size < size_cur) cur_ptr = &(parent -> lt_);
    else if(size > size_cur) cur_ptr = &(parent -> ge_);
    else
    {
      block_free -> ge_ = parent;
      parent = parent -> parent_;
      block_free -> ge_ -> parent_ = block_free;
      *cur_ptr = NULL;
    }
  }

  *cur_ptr = block_free;
  block_free -> parent_ = parent;
}

static void alloc_block(INTERNAL * internal, BLOCK_FREE * block)
{
  BLOCK_FREE * repl;

  if(block -> ge_ == NULL) repl = block -> lt_;
  else if(block -> lt_ == NULL) repl = block -> ge_;
  else
  {
    repl = block -> ge_;

    BLOCK_FREE ** cur_ptr = &(repl -> lt_);
    while(*cur_ptr != NULL) cur_ptr = &((*cur_ptr) -> lt_);
    *cur_ptr = block -> lt_;
  }

  if(repl != NULL) repl -> parent_ = block -> parent_;
  if(block -> parent_ -> ge_ == block) block -> parent_ -> ge_ = repl;
  else block -> parent_ -> lt_ = repl;
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

static void init_chunk(INTERNAL * internal, CHUNK * chunk, int size)
{
  chunk -> size_ = size;
  chunk -> prev_ = NULL;
  chunk -> next_ = NULL;

  BLOCK * block_list = chunk_block_list(chunk),
        * block_tail_list = chunk_block_tail_list(chunk);
  init_block(block_list);
  init_block(block_tail_list);

  add_block(block_tail_list, block_list);
  free_block(internal, block_list);
}

static CHUNK * create_chunk(INTERNAL * internal, int alloc_size)
{
  int chunk_size = internal -> chunk_size_;
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
    init_chunk(internal, chunk, chunk_size);

    if(internal -> chunk_tail_list_ != NULL)
    {
      internal -> chunk_tail_list_ -> next_ = chunk;
      chunk -> prev_ = internal -> chunk_tail_list_;
    }
    else
    {
      internal -> chunk_list_ = chunk;
    }
    internal -> chunk_tail_list_ = chunk;

    return chunk;
  }
}

/*******************************************************************************
*******************************************************************************/

static BLOCK_FREE * create_block_free(INTERNAL * internal, int alloc_size)
{
  CHUNK * chunk = create_chunk(internal, alloc_size);
  if(chunk == NULL) return NULL;

  return (BLOCK_FREE *)chunk_block_list(chunk);
}

/*******************************************************************************
*******************************************************************************/

static void * _alloc(CMAP_MEM * mem, int size)
{
  INTERNAL * internal = (INTERNAL *)mem -> internal_;

  BLOCK_FREE * block = find_block_free(internal, size);
  if(block == NULL) block = create_block_free(internal, size);
  if(block == NULL) return NULL;

  void * ret = ((void *)block) + sizeof(BLOCK);

  alloc_block(internal, block);

  int size_waste = block_size((BLOCK *)block) - size;
  if(size_waste >= sizeof(BLOCK_FREE))
  {
    ((BLOCK *)block) -> waste_ = 0;

    BLOCK * new_block = (BLOCK *)(ret + size);
    valid_block(new_block);
    add_block(new_block, (BLOCK *)block);
    free_block(internal, new_block);
  }
  else
  {
    ((BLOCK *)block) -> waste_ = size_waste;
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void _free(CMAP_MEM * mem, void * ptr)
{
  // TOCONTINUE
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM * create_mem(int chunk_size) {
  mem_.alloc = _alloc;
  mem_.free = _free;

  internal_.chunk_size_ =
    (chunk_size > CHUNK_SIZE_MIN) ? chunk_size : CHUNK_SIZE_MIN;
  internal_.chunk_list_ = NULL;
  internal_.chunk_tail_list_ = NULL;
  mem_.internal_ = &internal_;

  return &mem_;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MEM_FACTORY * cmap_mem_factory()
{
  mem_factory_.create = create_mem;
  return &mem_factory_;
}
