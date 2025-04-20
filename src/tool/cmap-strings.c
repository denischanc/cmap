
#include "cmap-strings.h"

#include <string.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_IMPL(strings, char *)

/*******************************************************************************
*******************************************************************************/

static char contains(CMAP_STRINGS * strings, const char * string)
{
  while(strings != NULL)
  {
    if(!strcmp(string, strings -> v)) return (1 == 1);
    strings = strings -> next;
  }
  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_STRINGS ** strings_ptr, const char * string)
{
  if(!contains(*strings_ptr, string))
    cmap_stack_strings_push(strings_ptr, strdup(string));
}

/*******************************************************************************
*******************************************************************************/

static void apply(CMAP_STRINGS * strings, CMAP_STRINGS_STRING_FN fn,
  void * data)
{
  if(strings == NULL) return;
  apply(strings -> next, fn, data);

  fn(strings -> v, data);
}

/*******************************************************************************
*******************************************************************************/

static void clone_string_fn(const char * string, void * data)
{
  add((CMAP_STRINGS **)data, string);
}

static CMAP_STRINGS * clone(CMAP_STRINGS * strings)
{
  CMAP_STRINGS * ret = NULL;
  apply(strings, clone_string_fn, &ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_STRINGS ** strings_ptr)
{
  while(*strings_ptr != NULL) free(cmap_stack_strings_pop(strings_ptr));
}

/*******************************************************************************
*******************************************************************************/

const CMAP_STRINGS_PUBLIC cmap_strings_public =
{
  contains,
  add,
  apply,
  clone,
  delete
};
