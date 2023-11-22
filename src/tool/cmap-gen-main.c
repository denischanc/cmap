
#include "cmap-gen-main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "cmap-file-util.h"
#include "cmap-option.h"
#include "cmap-parser-part.h"
#include "cmap-string.h"
#include "cmap-gen.h"

/*******************************************************************************
*******************************************************************************/

static const char * include = NULL;

/*******************************************************************************
*******************************************************************************/

static char * include_wo_ext()
{
  if(include == NULL) return NULL;
  else
  {
    char * ret = strdup(include);
    for(char * c = ret + (strlen(ret) - 1); c > ret; c--)
    {
      if(*c == '.')
      {
        *c = 0;
        return ret;
      }
    }
    return ret;
  }
}

static void impl(char ** txt)
{
  cmap_string_public.append(txt,
    "int main(int argc, char * argv[])\n"
    "{\n"
    "  cmap_bootstrap(NULL);\n"
    "  CMAP_ENV * env = cmap_env(argc, argv);\n");

  char * base_name = include_wo_ext();
  char * fn_name = cmap_gen_public.fn_name(base_name);
  free(base_name);
  if(fn_name != NULL)
  {
    cmap_string_public.append_args(txt,
      "  cmap_env_main(env, %s);\n", fn_name);
    free(fn_name);
  }

  cmap_string_public.append(txt,
    "  return cmap_main();\n"
    "}\n");
}

/*******************************************************************************
*******************************************************************************/

static void parts(char ** txt)
{
  cmap_parser_part_public.add_include("stdlib.h");
  cmap_parser_part_public.add_include("cmap-ext.h");
  if(include != NULL) cmap_parser_part_public.add_relative_include(include);

  cmap_string_public.append(txt, "\n");
  cmap_string_public.append(txt, cmap_parser_part_public.includes());
  cmap_string_public.append(txt, "\n");
  impl(txt);

  cmap_parser_part_public.clean();
}

/*******************************************************************************
*******************************************************************************/

static struct option gen_long_options[] =
{
  {"fn", required_argument, NULL, 'f'},
  {"relative-inc", no_argument, NULL, 'i'},
  {"include", required_argument, NULL, 'l'},
  {NULL, 0, NULL, 0}
};

static const char * gen_short_options = "f:il:";

static void mng_options(int argc, char * argv[])
{
  int o;
  while((o = getopt_long(argc, argv, gen_short_options, gen_long_options,
    NULL)) != -1)
  {
    switch(o)
    {
      case 'f': cmap_option_public.set_fn_name(optarg); break;
      case 'i': cmap_option_public.relative_inc(); break;
      case 'l': include = optarg; break;
    }
  }
}

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf(
    "usage: %s gen-main [file] (options)\n"
    "options:\n"
    "  -f,--fn [name]                       Function name\n"
    "  -i,--relative-inc                    Relative include\n"
    "  -l,--include [name]                  Include to add\n",
    this_name);
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if(argc < 3) usage(argv[0]);
  else
  {
    optind = 3;
    mng_options(argc, argv);

    char * txt = NULL;
    parts(&txt);
    int ret = cmap_file_util_public.to_file(argv[2], txt);
    free(txt);
    if(ret != 0) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_GEN_MAIN_PUBLIC cmap_gen_main_public =
{
  impl,
  main_
};
