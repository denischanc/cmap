
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

int cmap_strings_contains(CMAP_STRINGS * strings, const char * string)
{
  int ret = -1;
  if(strings != NULL) contains_helper(strings, string, &ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

char cmap_strings_add(CMAP_STRINGS ** strings_ptr, const char * string)
{
  if(cmap_strings_contains(*strings_ptr, string) < 0)
  {
    cmap_stack_strings_push(strings_ptr, strdup(string));
    return (1 == 1);
  }
  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static int set_helper(CMAP_STRINGS * strings, int off, const char * string)
{
  if(strings == NULL) return -1;

  int cur = set_helper(strings -> next, off, string) + 1;
  if(cur == off)
  {
    free(strings -> v);
    strings -> v = strdup(string);
  }

  return cur;
}

void cmap_strings_set(CMAP_STRINGS * strings, int off, const char * string)
{
  set_helper(strings, off, string);
}

/*******************************************************************************
*******************************************************************************/

void cmap_strings_apply(CMAP_STRINGS * strings, CMAP_STRINGS_STRING_FN fn,
  void * data)
{
  if(strings == NULL) return;
  cmap_strings_apply(strings -> next, fn, data);

  fn(strings -> v, data);
}

/*******************************************************************************
*******************************************************************************/

static void add_all_apply(const char * string, void * data)
{
  cmap_strings_add(data, string);
}

void cmap_strings_add_all(CMAP_STRINGS ** strings_ptr, CMAP_STRINGS * others)
{
  cmap_strings_apply(others, add_all_apply, strings_ptr);
}

/*******************************************************************************
*******************************************************************************/

CMAP_STRINGS * cmap_strings_split(const char * string)
{
  CMAP_STRINGS * ret = NULL;

  char * work = strdup(string), * begin = work, * end = work;
  while(1 == 1)
  {
    if((*end == ' ') || (*end == 0))
    {
      char c = *end;

      if(begin < end) { *end = 0; cmap_strings_add(&ret, begin); }

      if(c == 0) { free(work); return ret; }

      end++;
      begin = end;
    }
    else end++;
  }
}

/*******************************************************************************
*******************************************************************************/

void cmap_strings_delete(CMAP_STRINGS ** strings_ptr)
{
  while(*strings_ptr != NULL) free(cmap_stack_strings_pop(strings_ptr));
}
