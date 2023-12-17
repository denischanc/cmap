
#include "cmap-refsstore.h"

#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-set.h"
#include "cmap-stack.h"
#include "cmap-log.h"
#include "cmap-lifecycle.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_SET_lc * refs;

  int nb_created;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_REFSSTORE * this, CMAP_LIFECYCLE * lc, char created)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(cmap_set_lc_public.add(&internal -> refs, lc))
  {
    CMAP_INC_REFS(lc);

    if(created) internal -> nb_created++;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete_with_nested(CMAP_LIFECYCLE * lc)
{
  CMAP_STACK_lc_ptr * nested_stack = NULL;
  CMAP_CALL_ARGS(lc, nested, &nested_stack);
  while(nested_stack != NULL)
  {
    CMAP_LIFECYCLE ** cur = cmap_stack_lc_ptr_public.pop(&nested_stack);
    if(*cur != NULL)
    {
      CMAP_DEC_REFS(*cur);
      *cur = NULL;
    }
  }

  CMAP_CALL(lc, delete);
}

/*******************************************************************************
*******************************************************************************/

static void all_refs(CMAP_LIFECYCLE * lc, CMAP_LIFECYCLE * org,
  CMAP_STACK_lc_ptr ** refs, CMAP_SET_lc ** visit)
{
  if(cmap_set_lc_public.add(visit, lc))
  {
    CMAP_STACK_lc_ptr * nested_stack = NULL;
    CMAP_CALL_ARGS(lc, nested, &nested_stack);
    while(nested_stack != NULL)
    {
      CMAP_LIFECYCLE ** cur = cmap_stack_lc_ptr_public.pop(&nested_stack);
      if(*cur == org) cmap_stack_lc_ptr_public.push(refs, cur);
      else if(*cur != NULL) all_refs(*cur, org, refs, visit);
    }
  }
}

static CMAP_STACK_lc_ptr * get_refs(CMAP_LIFECYCLE * lc)
{
  CMAP_STACK_lc_ptr * refs = NULL;

  CMAP_SET_lc * visit = NULL;
  all_refs(lc, lc, &refs, &visit);
  cmap_set_lc_public.clean(&visit);

  return refs;
}

/*******************************************************************************
*******************************************************************************/

static char is_zombie(CMAP_LIFECYCLE * lc)
{
  char ret = CMAP_F;

  CMAP_STACK_lc_ptr * refs = get_refs(lc);
  if(refs != NULL)
  {
    ret = (CMAP_CALL(lc, nb_refs) == refs -> size);
    cmap_stack_lc_ptr_public.clean(&refs);
  }

  return ret;
}

static char delete_future_zombie_required(CMAP_LIFECYCLE * lc, int nb_refs)
{
  char ret = CMAP_T;

  CMAP_STACK_lc_ptr * refs = NULL;
  CMAP_SET_lc * visit = NULL;
  all_refs(lc, lc, &refs, &visit);

  if((refs != NULL) && (nb_refs == (refs -> size + 1)))
  {
    while(ret && (visit != NULL))
    {
      CMAP_LIFECYCLE * cur = cmap_set_lc_public.rm(&visit);
      if((cur != NULL) && (cur != lc) && !is_zombie(cur)) ret = CMAP_F;
    }
  }
  else ret = CMAP_F;

  if(ret) while(refs != NULL) *cmap_stack_lc_ptr_public.pop(&refs) = NULL;
  else if(refs != NULL) cmap_stack_lc_ptr_public.clean(&refs);

  if(visit != NULL) cmap_set_lc_public.clean(&visit);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char delete_or_dec_refs_only(CMAP_LIFECYCLE * lc, char delete_zombie)
{
  char do_delete = CMAP_F;

  int nb_refs = CMAP_CALL(lc, nb_refs);
  if(nb_refs == 1) do_delete = CMAP_T;
  else if(delete_zombie && delete_future_zombie_required(lc, nb_refs))
  {
    cmap_log_public.debug("[%p][%s] delete zombie", lc, CMAP_NATURE(lc));
    do_delete = CMAP_T;
  }

  if(do_delete) delete_with_nested(lc);
  else CMAP_CALL(lc, dec_refs_only);

  return do_delete;
}

/*******************************************************************************
*******************************************************************************/

static void delete_refs(INTERNAL * internal, CMAP_LIFECYCLE * ret)
{
  int nb_loop = 0, nb_deleted = 0;
  char fnd_ret = CMAP_F,
    delete_zombie = CMAP_KERNEL_INSTANCE -> cfg() -> delete_zombie;
  CMAP_SET_lc ** refs = &internal -> refs;
  while(*refs != NULL)
  {
    CMAP_LIFECYCLE * lc = cmap_set_lc_public.rm(refs);
    if(lc == ret) fnd_ret = CMAP_T;
    else if(delete_or_dec_refs_only(lc, delete_zombie)) nb_deleted++;
    nb_loop++;
  }
  cmap_log_public.debug("[refsstore] deleted %d/%d", nb_deleted, nb_loop);

  if(fnd_ret) CMAP_CALL(ret, dec_refs_only);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_REFSSTORE * this, CMAP_MAP * ret)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  cmap_log_public.debug("[%p][refsstore] created %d", this,
    internal -> nb_created);

  delete_refs(internal, (CMAP_LIFECYCLE *)ret);

  CMAP_KERNEL_FREE(internal);
  CMAP_KERNEL_FREE(this);
}

static CMAP_REFSSTORE * create()
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> refs = NULL;
  internal -> nb_created = 0;

  CMAP_KERNEL_ALLOC_PTR(this, CMAP_REFSSTORE);
  this -> internal = internal;
  this -> delete = delete;
  this -> add = add;

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_REFSSTORE_PUBLIC cmap_refsstore_public = { create };
