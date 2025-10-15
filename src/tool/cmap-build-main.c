
#include "cmap-build-main.h"

#include <stdlib.h>
#include <stdio.h>
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

static int usage(const char * this_name)
{
  printf(
    "usage: %s %s [file] (options)\n"
    "options:\n"
    "  -f,--fn [name]                       Function name\n"
    "  -i,--relative-inc                    Relative include\n"
    "  -l,--include [name]                  Include to add\n",
    this_name, CMAP_BUILD_MAIN_MODULE_NAME);

  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if(argc < 3) return usage(argv[0]);

  optind = 3;
  mng_options(argc, argv);

  cmap_fn_name_public.from_path_resolve(cmap_option_public.include());

  char * txt = NULL;
  parts(&txt);
  int ret = cmap_file_util_public.to_file(argv[2], txt);
  free(txt);

  cmap_clean_public.clean();

  return (ret != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_BUILD_MAIN_PUBLIC cmap_build_main_public = { impl, main_ };
