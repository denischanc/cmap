
#include "cmap-parser-process.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-parser-var.h"
#include "cmap-string.h"
#include "cmap-parser-util.h"
#include "cmap-part.h"

/*******************************************************************************
*******************************************************************************/

static char * process(char * map, char * fn_name, char * args)
{
  char * map_keep = (map == NULL) ? strdup("NULL") : strdup(map),
    * map_fn = (map == NULL) ? cmap_parser_var_public.name(fn_name) :
      cmap_parser_var_public.path(map, fn_name), * instruction = NULL;

  cmap_string_public.append_args(&instruction,
    "cmap_fn_proc((CMAP_FN *)%s, proc_ctx, %s", map_fn, map_keep);
  ADD_ARGS(&instruction, args);

  free(map_fn);
  free(map_keep);

  return instruction;
}

/*******************************************************************************
*******************************************************************************/

static char * process_fn(char * fn, char * args)
{
  char * instruction = NULL;

  cmap_string_public.append_args(&instruction,
    "cmap_fn_proc((CMAP_FN *)%s, proc_ctx, NULL", fn);
  ADD_ARGS(&instruction, args);

  free(fn);

  return instruction;
}

/*******************************************************************************
*******************************************************************************/

static char * process_instruction(char * txt, char need_ret)
{
  char * map_name = NULL;

  if(need_ret)
  {
    map_name = NEXT_NAME("var");
    PREPEND_MAP_VAR(map_name);

    APPEND_INSTRUCTION_ARGS("%s = %s", map_name, txt);
  }
  else APPEND_INSTRUCTION(txt);
  APPEND_LF();

  free(txt);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_c(char * fn_name, char need_ret)
{
  char * map_name = NULL;

  char * proc_ctx_name = NEXT_NAME("proc_ctx");
  APPEND_INSTRUCTION_ARGS(
    "CMAP_PROC_CTX * %s = cmap_proc_ctx(proc_ctx);", proc_ctx_name);

  if(need_ret)
  {
    map_name = NEXT_NAME("var");
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

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void return_(char * map)
{
  if(map == NULL) APPEND_INSTRUCTION(
    cmap_part_public.is_return_fn() ? "return NULL;" : "return;");
  else
  {
    APPEND_INSTRUCTION_ARGS("return %s;", map);
    free(map);

    cmap_part_public.return_fn();
    cmap_part_public.return_();
  }
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_PROCESS_PUBLIC cmap_parser_process_public =
  {process, process_fn, process_instruction, process_c, return_};
