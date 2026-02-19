#ifndef __CMAP_STRING_H__
#define __CMAP_STRING_H__

#include <stdarg.h>

void cmap_string_append(char ** src, const char * txt);
void cmap_string_vappend_args(char ** src, const char * txt, va_list args);
void cmap_string_append_args(char ** src, const char * txt, ...);
void cmap_string_prepend(char ** src, const char * txt);

#endif
