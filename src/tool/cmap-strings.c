
#include "cmap-strings.h"

#include <string.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_IMPL(strings, char *)

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_STRINGS ** strings_ptr, const char * string)
{
  cmap_stack_strings_push(strings_ptr, strdup(string));
}

/*******************************************************************************
*******************************************************************************/

static void add_strings(CMAP_STRINGS ** strings_ptr, CMAP_STRINGS * strings)
{
  while(strings != NULL)
  {
    add(strings_ptr, strings -> v);
    strings = strings -> next;
  }
}

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

static void delete(CMAP_STRINGS ** strings_ptr)
{
  while(*strings_ptr != NULL) free(cmap_stack_strings_pop(strings_ptr));
}

/*******************************************************************************
*******************************************************************************/

const CMAP_STRINGS_PUBLIC cmap_strings_public =
{
  add,
  add_strings,
  contains,
  delete
};
