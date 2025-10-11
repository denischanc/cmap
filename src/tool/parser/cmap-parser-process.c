
#include "cmap-parser-process.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-parser-var.h"
#include "cmap-string.h"
#include "cmap-parser-util.h"
#include "cmap-part.h"

/*******************************************************************************
*******************************************************************************/

static char * process_append(char * map_fn, char * map, char * args,
  char need_map_name)
{
  char * map_name = need_map_name ? NEXT_NAME_VAR() : NULL;

  char * instruction = strdup("");
  if(map_name != NULL)
  {
    PREPEND_MAP_VAR(map_name);
    cmap_string_public.append_args(&instruction, "%s = ", map_name);
  }

  APPEND_INSTRUCTION_ARGS_ARGS(args,
    "%scmap_fn_proc((CMAP_FN *)%s, proc_ctx, %s", instruction, map_fn, map);
  APPEND_LF();

  free(map_fn);
  free(map);
  free(instruction);

  cmap_part_public.var.proc_clean();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process(char * map, char * fn_name, char * args,
  char need_map_name)
{
  char * map_keep = (map == NULL) ? strdup("NULL") : strdup(map),
    * map_fn = (map == NULL) ? cmap_parser_var_public.name(fn_name) :
      cmap_parser_var_public.path(map, fn_name);
  return process_append(map_fn, map_keep, args, need_map_name);
}

/*******************************************************************************
*******************************************************************************/

static char * process_fn(char * fn, char * args, char need_map_name)
{
  return process_append(fn, strdup("NULL"), args, need_map_name);
}

/*******************************************************************************
*******************************************************************************/

static char * process_c(char * fn_name, char need_map_name)
{
  char * map_name = NULL;

  char * proc_ctx_name = NEXT_NAME_PROC_CTX();
  APPEND_INSTRUCTION_ARGS(
    "CMAP_PROC_CTX * %s = cmap_proc_ctx(proc_ctx);", proc_ctx_name);

  if(need_map_name)
  {
    map_name = NEXT_NAME_VAR();
    PREPEND_MAP_VAR(map_name);
    APPEND_INSTRUCTION_ARGS("%s = cmap_delete_proc_ctx(%s, %s(%s));",
      map_name, proc_ctx_name, fn_name, proc_ctx_name);
  }
  else
  {
    APPEND_INSTRUCTION_ARGS("%s(%s);", fn_name, proc_ctx_name);
    APPEND_INSTRUCTION_ARGS("cmap_delete_proc_ctx(%s, NULL);", proc_ctx_name);
  }
  APPEND_LF();

  free(fn_name);
  free(proc_ctx_name);

  cmap_part_public.var.proc_clean();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void return_(char * map)
{
  if(map == NULL) APPEND_INSTRUCTION(
    cmap_part_public.ctx.return_fn() ? "return NULL;" : "return;");
  else
  {
    APPEND_INSTRUCTION_ARGS("return %s;", map);
    free(map);

    cmap_part_public.ctx.set_return();
  }
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_PROCESS_PUBLIC cmap_parser_process_public =
  {process, process_fn, process_c, return_};
