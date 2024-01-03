
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
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(cmap_set_lc_public.add(&internal -> refs, lc))
  {
    CMAP_INC_REFS(lc);

    if(created) internal -> nb_created++;
  }
}

/*******************************************************************************
*******************************************************************************/

static void dec_nested_refs_apply(CMAP_LIFECYCLE ** lc, void * data)
{
  if(*lc != NULL)
  {
    CMAP_DEC_REFS(*lc);
    *lc = NULL;
  }
}

static void delete_with_nested(CMAP_LIFECYCLE * lc)
{
  CMAP_STACK_LC_PTR * nested_stack = cmap_stack_lc_ptr_public.create(0);
  CMAP_CALL_ARGS(lc, nested, nested_stack);
  CMAP_CALL_ARGS(nested_stack, apply, dec_nested_refs_apply, NULL);
  CMAP_CALL(nested_stack, delete);

  CMAP_CALL(lc, delete);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * org;
  CMAP_STACK_LC_PTR * crefs;
  CMAP_SET_lc ** visit;
} ALL_CREFS_APPLY_DATA;

static void all_cycling_refs(CMAP_LIFECYCLE * lc, CMAP_LIFECYCLE * org,
  CMAP_STACK_LC_PTR * crefs, CMAP_SET_lc ** visit);

static void all_crefs_apply(CMAP_LIFECYCLE ** lc, void * data)
{
  ALL_CREFS_APPLY_DATA * data_ = (ALL_CREFS_APPLY_DATA *)data;
  if(*lc == data_ -> org) CMAP_CALL_ARGS(data_ -> crefs, push, lc);
  else if(*lc != NULL) all_cycling_refs(*lc, data_ -> org, data_ -> crefs,
    data_ -> visit);
}

static void all_cycling_refs(CMAP_LIFECYCLE * lc, CMAP_LIFECYCLE * org,
  CMAP_STACK_LC_PTR * crefs, CMAP_SET_lc ** visit)
{
  if(cmap_set_lc_public.add(visit, lc))
  {
    CMAP_STACK_LC_PTR * nested_stack = cmap_stack_lc_ptr_public.create(0);
    CMAP_CALL_ARGS(lc, nested, nested_stack);
    ALL_CREFS_APPLY_DATA data = { org, crefs, visit };
    CMAP_CALL_ARGS(nested_stack, apply, all_crefs_apply, &data);
    CMAP_CALL(nested_stack, delete);
  }
}

static CMAP_STACK_LC_PTR * get_cycling_refs(CMAP_LIFECYCLE * lc)
{
  CMAP_STACK_LC_PTR * crefs = cmap_stack_lc_ptr_public.create(0);

  CMAP_SET_lc * visit = NULL;
  all_cycling_refs(lc, lc, crefs, &visit);
  cmap_set_lc_public.clean(&visit);

  return crefs;
}

/*******************************************************************************
*******************************************************************************/

static char stay_future_zombie(CMAP_LIFECYCLE * visited)
{
  CMAP_STACK_LC_PTR * crefs = get_cycling_refs(visited);
  int crefs_size = CMAP_CALL(crefs, size);
  CMAP_CALL(crefs, delete);
  return ((crefs_size == 0) || (CMAP_CALL(visited, nb_refs) == crefs_size));
}

static void zombie_crefs_apply(CMAP_LIFECYCLE ** lc, void * data)
{
  *lc = NULL;
}

static char delete_future_zombie_required(CMAP_LIFECYCLE * lc, int nb_refs)
{
  char ret = CMAP_T;

  CMAP_STACK_LC_PTR * crefs = cmap_stack_lc_ptr_public.create(0);
  CMAP_SET_lc * visit = NULL;
  all_cycling_refs(lc, lc, crefs, &visit);

  if(nb_refs == (CMAP_CALL(crefs, size) + 1))
  {
    while(ret && (visit != NULL))
    {
      CMAP_LIFECYCLE * visited = cmap_set_lc_public.rm(&visit);
      if((visited != NULL) && (visited != lc) && !stay_future_zombie(visited))
        ret = CMAP_F;
    }
  }
  else ret = CMAP_F;

  if(ret) CMAP_CALL_ARGS(crefs, apply, zombie_crefs_apply, NULL);

  CMAP_CALL(crefs, delete);

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
  INTERNAL * internal = (INTERNAL *)(this + 1);

  cmap_log_public.debug("[%p][refsstore] created %d", this,
    internal -> nb_created);

  delete_refs(internal, (CMAP_LIFECYCLE *)ret);

  CMAP_KERNEL_FREE(this);
}

static CMAP_REFSSTORE * create()
{
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_REFSSTORE * this = (CMAP_REFSSTORE *)mem -> alloc(
    sizeof(CMAP_REFSSTORE) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> refs = NULL;
  internal -> nb_created = 0;

  this -> delete = delete;
  this -> add = add;

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_REFSSTORE_PUBLIC cmap_refsstore_public = { create };
