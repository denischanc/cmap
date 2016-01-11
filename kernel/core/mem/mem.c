
#include "mem.h"

#include <stdlib.h>
#include <stdio.h>

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
  block -> free_ = (1 == 0);
  block -> prev_ = NULL;
  block -> next_ = NULL;
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
  block_list -> next_ = block_tail_list;
  block_tail_list -> prev_ = block_list;
}

static CHUNK * new_chunk(INTERNAL * internal, int alloc_size)
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

static BLOCK * find_block_free(INTERNAL * internal, int alloc_size)
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
  return (BLOCK *)result;
}

static void add_block_free(INTERNAL * internal, BLOCK_FREE * block)
{
  BLOCK_FREE ** parent = &internal -> block_free_tree_;
  int size = block_size((BLOCK *)block), size_parent;

  while(*parent != NULL)
  {
    size_parent = block_size((BLOCK *)*parent);
    if(size < size_parent) parent = &((*parent) -> lt_);
    else if(size > size_parent) parent = &((*parent) -> ge_);
    else
    {
      /* TOCONTINUE */
    }
  }
}

static void set_block_free(INTERNAL * internal, BLOCK * block)
{
  block -> free_ = (1 == 1);

  add_block_free(internal, (BLOCK_FREE *)block);
}

static BLOCK * new_block_free(INTERNAL * internal, int alloc_size)
{
  CHUNK * chunk = new_chunk(internal, alloc_size);
  if(chunk == NULL) return NULL;

  BLOCK * block = chunk_block_list(chunk);
  set_block_free(internal, block);

  return block;
}

/*******************************************************************************
*******************************************************************************/

static void * _alloc(CMAP_MEM * mem, int size)
{
  INTERNAL * internal = (INTERNAL *)mem -> internal_;

  BLOCK * block = find_block_free(internal, size);
  if(block == NULL) block = new_block_free(internal, size);
  if(block == NULL) return NULL;

  /* TOCONTINUE
  alloc_block(block);*/

  return ((void *)block) + sizeof(BLOCK);
}

/*******************************************************************************
*******************************************************************************/

static void _free(CMAP_MEM * mem, void * ptr)
{
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
