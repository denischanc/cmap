
#include "cmap-build-main.h"

#include <stdlib.h>
#include "cmap-file-util.h"
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-fn-name.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

static void impl(char ** txt)
{
  cmap_string_public.append(txt,
    "int main(int argc, char * argv[])\n"
    "{\n");

  const char * fn_name = cmap_config_public.fn();
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
  if(cmap_config_public.include() != NULL)
    cmap_part_public.add_include(cmap_config_public.include(), (1 == 1));

  cmap_string_public.append(txt, "\n");
  cmap_string_public.append(txt, *cmap_part_public.includes());
  cmap_string_public.append(txt, "\n");
  impl(txt);
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if(argc < 2)
  {
    int ids[] = {CMAP_CONFIG_ID_FN, CMAP_CONFIG_ID_RELATIVE_INC,
      CMAP_CONFIG_ID_INCLUDE, 0};
    return cmap_config_public.usage(CMAP_BUILD_MAIN_MODULE_NAME " [file]", ids);
  }

  cmap_fn_name_public.resolve_to_config(cmap_config_public.include());

  char * txt = NULL;
  parts(&txt);
  char ret = cmap_file_util_public.to_file(argv[1], txt);
  free(txt);

  cmap_part_public.clean();

  return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_BUILD_MAIN_PUBLIC cmap_build_main_public = {impl, main_};
