
#include "cmap-util.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_util_vfill_list(CMAP_LIST * list, va_list maps)
{
  CMAP_MAP * map;
  while((map = va_arg(maps, CMAP_MAP *)) != NULL) CMAP_LIST_PUSH(list, map);
  return list;
}

CMAP_LIST * cmap_util_fill_list(CMAP_LIST * list, ...)
{
  va_list maps;
  va_start(maps, list);
  cmap_util_vfill_list(list, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_util_vto_list(CMAP_PROC_CTX * proc_ctx, va_list maps)
{
  return cmap_util_vfill_list(CMAP_LIST(0, proc_ctx), maps);
}

CMAP_LIST * cmap_util_to_list(CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list maps;
  va_start(maps, proc_ctx);
  CMAP_LIST * list = cmap_util_vto_list(proc_ctx, maps);
  va_end(maps);
  return list;
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_util_split(CMAP_PROC_CTX * proc_ctx, char * string, char sep)
{
  CMAP_LIST * ret = CMAP_LIST(0, proc_ctx);

  char * cur = string;
  while(CMAP_T)
  {
    char c = *cur;
    if((c == sep) || (c == 0))
    {
      *cur = 0;
      CMAP_LIST_PUSH(ret, CMAP_STRING(string, 0, proc_ctx));

      if(c == 0) return ret;
      *cur = sep;

      cur++;
      string = cur;
    }
    else cur++;
  }
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_util_vto_map(CMAP_PROC_CTX * proc_ctx, va_list key_maps)
{
  CMAP_MAP * ret = CMAP_MAP(proc_ctx);

  const char * key;
  while((key = va_arg(key_maps, char *)) != NULL)
  {
    CMAP_MAP * map = va_arg(key_maps, CMAP_MAP *);
    CMAP_SET(ret, key, map);
  }

  return ret;
}

CMAP_MAP * cmap_util_to_map(CMAP_PROC_CTX * proc_ctx, ...)
{
  va_list key_maps;
  va_start(key_maps, proc_ctx);
  CMAP_MAP * map = cmap_util_vto_map(proc_ctx, key_maps);
  va_end(key_maps);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static void copy_apply(const char * key, CMAP_MAP ** val, void * data)
{
  CMAP_SET(data, key, *val);
}

CMAP_MAP * cmap_util_copy(CMAP_MAP * dst, CMAP_MAP * src)
{
  if((dst != NULL) && (src != NULL))
    CMAP_CALL_ARGS(src, apply, copy_apply, dst);
  return dst;
}

/*******************************************************************************
*******************************************************************************/

char * cmap_util_strdup(const char * src)
{
  int size = (strlen(src) + 1) * sizeof(char);
  char * dst = cmap_mem_alloc(size);
  memcpy(dst, src, size);
  return dst;
}

/*******************************************************************************
*******************************************************************************/

uint64_t cmap_util_time_us()
{
  struct timeval tv = {0};
  gettimeofday(&tv, NULL);

  uint64_t i = tv.tv_sec;
  i *= 1000000;
  i += tv.tv_usec;

  return i;
}
