#ifndef __CMAP_COMPILE_H__
#define __CMAP_COMPILE_H__

#define CMAP_COMPILE_MODULE_NAME "compile"
#define CMAP_COMPILE_MODULE_MODULE_NAME "compile-module"

char * cmap_compile_exec_path(const char * cmap_path);

int cmap_compile_main(int argc, char * argv[]);

#endif
