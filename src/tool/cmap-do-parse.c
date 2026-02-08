
#include "cmap-do-parse.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cmap-part.h"
#include "cmap-parser-block.h"
#include "cmap-scanner.h"
#include "cmap-parser.h"
#include "cmap-console.h"

/*******************************************************************************
*******************************************************************************/

static const char * path__ = NULL;

/*******************************************************************************
*******************************************************************************/

static const char * path_()
{
  return path__;
}

/*******************************************************************************
*******************************************************************************/

static void clean()
{
  cmap_part_public.ctx.clean();
  cmap_parser_block_public.clean();
}

static char parse(const char * path)
{
  char * path_r = realpath(path, NULL);
  if(path_r == NULL)
  {
    cmap_console_public.error("[%s] %s\n", path, strerror(errno));
    return (1 == 0);
  }

  const char * path_bup = path__;
  path__ = path_r;

  FILE * in = fopen(path_r, "r");
  if(in == NULL)
  {
    cmap_console_public.error("[%s] %s\n", path_r, strerror(errno));
    path__ = path_bup;
    free(path_r);
    return (1 == 0);
  }

  yyscan_t scanner;
  cmap_parser_lex_init(&scanner);
  cmap_parser_set_in(in, scanner);

  int ret = cmap_parser_parse(scanner);
  if(ret != 0) clean();

  cmap_parser_lex_destroy(scanner);

  fclose(in);

  path__ = path_bup;
  free(path_r);

  return (ret == 0);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_DO_PARSE_PUBLIC cmap_do_parse_public = {path_, parse};
