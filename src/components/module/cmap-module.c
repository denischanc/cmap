
#include "cmap-module.h"

#include <stdlib.h>
#include <dlfcn.h>
#include "cmap.h"
#include "cmap-log.h"
#include "cmap-proc-ctx.h"
#include "cmap-config.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

#define MODULE_SYM "cmap_module"

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_module_load(const char * path_so, CMAP_PROC_CTX * proc_ctx)
{
  void * module = dlopen(path_so, RTLD_LAZY);
  if(module == NULL)
  {
    cmap_log_fatal("Unable to open '%s' : %s", path_so, dlerror());
    return NULL;
  }

  CMAP_MAP * (*fn)(CMAP_PROC_CTX *) = dlsym(module, MODULE_SYM);
  if(fn == NULL)
  {
    cmap_log_fatal("Unable to find '%s' into '%s' : %s",
      MODULE_SYM, path_so, dlerror());
    return NULL;
  }

  CMAP_PROC_CTX * new_proc_ctx = CMAP_CALL(proc_ctx, create);
  return CMAP_CALL_ARGS(new_proc_ctx, delete, fn(new_proc_ctx));
}

/*******************************************************************************
*******************************************************************************/

void cmap_module_load_from_config(CMAP_PROC_CTX * proc_ctx)
{
  char * modules = cmap_config_modules();
  if(modules == NULL) return;

  CMAP_LIST * modules_ = cmap_util_split(proc_ctx, modules, ' ');
  CMAP_STRING * module;
  while((module = (CMAP_STRING *)CMAP_LIST_SHIFT(modules_)) != NULL)
  {
    char * path_so = CMAP_CALL(module, val);
    if(path_so[0] != 0) cmap_module_load(path_so, proc_ctx);
  }
}
