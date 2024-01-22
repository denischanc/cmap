
#include "cmap-build-main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "cmap-file-util.h"
#include "cmap-option.h"
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-fn-name.h"
#include "cmap-clean.h"

/*******************************************************************************
*******************************************************************************/

static void impl(char ** txt)
{
  cmap_string_public.append(txt,
    "int main(int argc, char * argv[])\n"
    "{\n");

  const char * fn_name = cmap_fn_name_public.name();
  if(fn_name != NULL) cmap_string_public.append_args(txt,
    SPACE "cmap_env_main(cmap_env(argc, argv), %s);\n", fn_name);
  else cmap_string_public.append(txt, SPACE "cmap_env(argc, argv);\n");

  cmap_string_public.append(txt,
    SPACE "return cmap_main();\n"
    "}\n");
}

/*******************************************************************************
*******************************************************************************/

static void parts(char ** txt)
{
  cmap_part_public.add_include("stdlib.h", (1 == 0));
  cmap_part_public.add_include("cmap-ext.h", (1 == 0));
  if(cmap_option_public.include() != NULL)
    cmap_part_public.add_include(cmap_option_public.include(), (1 == 1));

  cmap_string_public.append(txt, "\n");
  cmap_string_public.append(txt, *cmap_part_public.includes());
  cmap_string_public.append(txt, "\n");
  impl(txt);

  cmap_clean_public.clean();
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
      case 'f': cmap_fn_name_public.from_option(optarg); break;
      case 'i': cmap_option_public.relative_inc(); break;
      case 'l': cmap_option_public.set_include(optarg); break;
    }
  }
}

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf(
    "usage: %s %s [file] (options)\n"
    "options:\n"
    "  -f,--fn [name]                       Function name\n"
    "  -i,--relative-inc                    Relative include\n"
    "  -l,--include [name]                  Include to add\n",
    this_name, CMAP_BUILD_MAIN_MODULE_NAME);
}

/*******************************************************************************
*******************************************************************************/

static char * include_no_suffix()
{
  if(cmap_option_public.include() == NULL) return NULL;
  else
  {
    char * ret = strdup(cmap_option_public.include());
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

static int main_(int argc, char * argv[])
{
  if(argc < 3) usage(argv[0]);
  else
  {
    optind = 3;
    mng_options(argc, argv);

    char * include_name = include_no_suffix();
    cmap_fn_name_public.from_basename_no_suffix(include_name);
    free(include_name);

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

const CMAP_BUILD_MAIN_PUBLIC cmap_build_main_public = { impl, main_ };
