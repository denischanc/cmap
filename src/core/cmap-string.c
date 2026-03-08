
#include "cmap-string.h"

#include <string.h>
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-log.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_STRING_NATURE = "string";

/*******************************************************************************
*******************************************************************************/

char * cmap_string_val(CMAP_STRING * string)
{
  return string -> internal.val;
}

/*******************************************************************************
*******************************************************************************/

static inline int adjusted_size_max(CMAP_STRING * string, int size)
{
  int size_inc = string -> internal.size_inc,
    new_size = string -> internal.size + size,
    remainder = new_size % size_inc;
  if(remainder == 0) return new_size;
  else return new_size + size_inc - remainder;
}

/*******************************************************************************
*******************************************************************************/

static void append(CMAP_STRING * string, const char * val, int size_append)
{
  int off = string -> internal.size - 1,
    new_size_max = adjusted_size_max(string, size_append);

  if(new_size_max > string -> internal.size_max)
  {
    char * new_val = cmap_mem_alloc(new_size_max),
      * old_val = string -> internal.val;
    memcpy(new_val, old_val, off);
    cmap_mem_free(old_val);

    string -> internal.val = new_val;
    string -> internal.size_max = new_size_max;
  }

  int new_size = string -> internal.size + size_append;
  char * int_val = string -> internal.val;
  memcpy(int_val + off, val, size_append);
  int_val[new_size - 1] = 0;

  string -> internal.size = new_size;
}

/*******************************************************************************
*******************************************************************************/

void cmap_string_append(CMAP_STRING * string, const char * val)
{
  append(string, val, strlen(val));
}

/*******************************************************************************
*******************************************************************************/

void cmap_string_append_sub(CMAP_STRING * string, const char * val,
  int off_start, int off_stop)
{
  append(string, val + off_start, off_stop - off_start);
}

/*******************************************************************************
*******************************************************************************/

void cmap_string_clean(CMAP_STRING * string)
{
  string -> internal.val[0] = 0;
  string -> internal.size = 1;
}

/*******************************************************************************
*******************************************************************************/

void cmap_string_delete(CMAP_LIFECYCLE * lc)
{
  cmap_mem_free(((CMAP_STRING *)lc) -> internal.val);

  cmap_map_delete(lc);
}

CMAP_STRING * cmap_string_init(CMAP_STRING * string, CMAP_INITARGS * initargs,
  const char * val, int size_inc)
{
  cmap_map_init((CMAP_MAP *)string, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)string;
  lc -> delete = cmap_string_delete;

  if(size_inc < cmap_config_core_string_size_inc_min())
    size_inc = cmap_config_core_string_size_inc();
  string -> internal.size_inc = size_inc;
  string -> internal.size = strlen(val) + 1;
  string -> internal.size_max = adjusted_size_max(string, 0);
  string -> internal.val = cmap_mem_alloc(string -> internal.size_max);
  memcpy(string -> internal.val, val, string -> internal.size);

  cmap_log_debug("[%p][%s] => [%.60s]", string, CMAP_STRING_NATURE, val);

  return string;
}

CMAP_STRING * cmap_string_create(const char * val, int size_inc,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);
  initargs.nature = CMAP_STRING_NATURE;
  initargs.prototype = cmap_prototypestore_string(ps, proc_ctx);
  initargs.proc_ctx = proc_ctx;

  return cmap_string_init(CMAP_MEM_ALLOC(CMAP_STRING), &initargs, val,
    size_inc);
}
