
#include "cmap-parser-process.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "cmap-parser-var.h"
#include "cmap-string.h"
#include "cmap-parser-util.h"
#include "cmap-part.h"
#include "cmap-fn-name.h"
#include "cmap-config.h"
#include "cmap-do-parse.h"

/*******************************************************************************
*******************************************************************************/

static char * process_append(char * map_fn, char * map, char * args,
  char do_return)
{
  char * map_name = do_return ? NEXT_NAME_VAR() : NULL;

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

static char * process(char * map, char * fn_name, char * args, char do_return)
{
  char * map_keep = (map == NULL) ? strdup("NULL") : strdup(map),
    * map_fn = (map == NULL) ? cmap_parser_var_public.name(fn_name) :
      cmap_parser_var_public.path(map, fn_name);
  return process_append(map_fn, map_keep, args, do_return);
}

/*******************************************************************************
*******************************************************************************/

static char * process_fn(char * fn, char * args, char do_return)
{
  return process_append(fn, strdup("NULL"), args, do_return);
}

/*******************************************************************************
*******************************************************************************/

static char * process_c(char * fn_name, char do_return)
{
  char * map_name = NULL;

  char * proc_ctx_name = NEXT_NAME_PROC_CTX();
  APPEND_INSTRUCTION_ARGS(
    "CMAP_PROC_CTX * %s = cmap_proc_ctx(proc_ctx);", proc_ctx_name);

  if(do_return)
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

typedef struct
{
  char * fn_name, only_c;
  CMAP_PART_CTX * ctx;
} IMPORT_CTX;

static IMPORT_CTX import_ctx_bup(const char * fn_name)
{
  IMPORT_CTX ctx;
  ctx.fn_name = strdup(cmap_config_public.fn());
  ctx.only_c = cmap_config_public.is_only_c();
  ctx.ctx = cmap_part_public.ctx.bup();

  cmap_config_public.set_fn(fn_name);
  cmap_config_public.set_only_c(1 == 1);

  return ctx;
}

static void import_ctx_restore(IMPORT_CTX ctx)
{
  cmap_config_public.set_fn(ctx.fn_name);
  free(ctx.fn_name);
  cmap_config_public.set_only_c(ctx.only_c);
  cmap_part_public.ctx.restore(ctx.ctx);
}

/*******************************************************************************
*******************************************************************************/

static char * import_parse_path(const char * path)
{
  if(path[0] == '/') return strdup(path);

  char * parse_path = strdup(cmap_do_parse_public.path());
  char * tmp = strrchr(parse_path, '/');
  if(tmp != NULL) *(tmp + 1) = 0;
  else { free(parse_path); parse_path = NULL; }

  cmap_string_public.append(&parse_path, path);

  tmp = realpath(parse_path, NULL);
  if(tmp == NULL) fprintf(stderr, "[%s] %s\n", parse_path, strerror(errno));
  free(parse_path);

  return tmp;
}

/*******************************************************************************
*******************************************************************************/

static char import(char ** map_name, char * path, char * fn_name)
{
  const char * path_ok = path + 1;
  path[strlen(path) - 1] = 0;

  if(fn_name == NULL) fn_name = cmap_fn_name_public.resolve(path_ok);
  char * parse_path = import_parse_path(path_ok);

  if(parse_path == NULL) { free(path); free(fn_name); return (1 == 0); }

  free(path);

  IMPORT_CTX bup = import_ctx_bup(fn_name);
  char ret = cmap_do_parse_public.parse(parse_path);
  free(parse_path);
  import_ctx_restore(bup);

  if(!ret) { free(fn_name); return (1 == 0); }

  if(map_name == NULL) process_c(fn_name, (1 == 0));
  else *map_name = process_c(fn_name, (1 == 1));
  return (1 == 1);
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
{
  process, process_fn,
  process_c, import,
  return_
};
