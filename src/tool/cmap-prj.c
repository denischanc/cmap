
#include "cmap-prj.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "cmap-file-util.h"
#include "config.h"

/*******************************************************************************
*******************************************************************************/

static const char * hello_world_cmap =
  "\n"
  "@FUNCTION(hello_world)\n"
  "{\n"
  "  LOCAL name = cmap.cli.args[[1]];\n"
  "  if(name == null) { name = \"World\"#t#; }\n"
  "  LOCAL max = 3#t#.valueOf(cmap.cli.args[[2]]);\n"
  "\n"
  "  LOCAL hello = function(name)#t#\n"
  "  {\n"
  "    cmap.console.info(\"Hello \"#t#, name, \" !!!\"#t#);\n"
  "  };\n"
  "\n"
  "  LOCAL helloWorld = function()#t#\n"
  "  {\n"
  "    if(this.nb == null) { this.nb = 1; }\n"
  "\n"
  "    hello(name);\n"
  "\n"
  "    if(this.nb >= max) { this.deepDeleteNoRef(); return; }\n"
  "    else { this.nb.inc(); }\n"
  "\n"
  "    this.schedule();\n"
  "  };\n"
  "\n"
  "  LOCAL job = new cmap.scheduler.job{helloWorld};\n"
  "  job.schedule();\n"
  "}\n"
  "\n"
  "[C[\n"
  "\n"
  "int main(int argc, char * argv[])\n"
  "{\n"
  "  cmap_bootstrap(NULL);\n"
  "  cmap_env_main(cmap_env(argc, argv), hello_world);\n"
  "  return cmap_main();\n"
  "}\n"
  "\n"
  "]]\n";

/*******************************************************************************
*******************************************************************************/

const char * makefile =
  "\n"
  ".PHONY: clean\n"
  "\n"
  "CMAP_TOOL = " CMAP_INSTALL_BINDIR "/cmap-tool\n"
  "\n"
  "CFLAGS = -Wall -I" CMAP_INSTALL_INCLUDEDIR "\n"
  "LDFLAGS = -L" CMAP_INSTALL_LIBDIR " -lcmap\n"
  "\n"
  "hello-world: hello-world.c\n"
  "\tgcc $(CFLAGS) $(LDFLAGS) -o $@ $^\n"
  "\n"
  "hello-world.c: hello-world.cmap $(CMAP_TOOL)\n"
  "\t$(CMAP_TOOL) gen $< $(@:.c=) --only-c\n"
  "\n"
  "clean:\n"
  "\trm -f hello-world hello-world.c\n";

/*******************************************************************************
*******************************************************************************/

static int to_file(const char * dir, const char * file_name, const char * txt)
{
  char buffer[10000];
  snprintf(buffer, sizeof(buffer), "%s/%s", dir, file_name);
  return cmap_file_util_public.to_file(buffer, txt);
}

/*******************************************************************************
*******************************************************************************/

static void usage(const char * this_name)
{
  printf("usage: %s prj [project directory]\n", this_name);
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  if(argc < 3) usage(argv[0]);
  else
  {
    char * dir = argv[2];
    if(mkdir(dir, 0755) < 0)
    {
      fprintf(stderr, "[%s] %s\n", dir, strerror(errno));
      return EXIT_FAILURE;
    }

    if(to_file(dir, "hello-world.cmap", hello_world_cmap) != 0)
      return EXIT_FAILURE;
    if(to_file(dir, "Makefile", makefile) != 0) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PRJ_PUBLIC cmap_prj_public =
{
  main_
};
