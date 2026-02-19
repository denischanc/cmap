#ifndef __CMAP_FN_NAME_H__
#define __CMAP_FN_NAME_H__

char * cmap_fn_name_resolve(const char * path);

void cmap_fn_name_to_config(const char * path);
void cmap_fn_name_to_config_when_null(const char * path);

#endif
