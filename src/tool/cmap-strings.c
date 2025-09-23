
#include "cmap-strings.h"

#include <string.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_IMPL(STRINGS, strings, char *)

/*******************************************************************************
*******************************************************************************/

static int contains_helper(CMAP_STRINGS * strings, const char * string,
  int * ret)
{
  CMAP_STRINGS * next = strings -> next;
  int i = (next == NULL) ? 0 : contains_helper(next, string, ret);

  if(!strcmp(strings -> v, string)) *ret = i;

  return i + 1;
}

static int contains(CMAP_STRINGS * strings, const char * string)
{
  int ret = -1;
  if(strings != NULL) contains_helper(strings, string, &ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char add(CMAP_STRINGS ** strings_ptr, const char * string)
{
  if(contains(*strings_ptr, string) < 0)
  {
    cmap_stack_strings_push(strings_ptr, strdup(string));
    return (1 == 1);
  }
  return (1 == 0);
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

static void add_all_apply(const char * string, void * data)
{
  add(data, string);
}

static void add_all(CMAP_STRINGS ** strings_ptr, CMAP_STRINGS * others)
{
  apply(others, add_all_apply, strings_ptr);
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
  add_all,
  delete
};
