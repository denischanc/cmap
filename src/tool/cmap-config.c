
#include "cmap-config.h"

#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "cmap-cli.h"

/*******************************************************************************
*******************************************************************************/

#define BOOL_VAR(name, ENV_VAR) \
  static char name##_val_ok = (1 == 0), name##_val = (1 == 0);

#define STRING_VAR(name, ENV_VAR, dft) \
  static char name##_val_ok = (1 == 0), * name##_val = NULL;

#define STRINGS_VAR(name, ENV_VAR) \
  static char name##_val_ok = (1 == 0); \
  static CMAP_STRINGS * name##_val = NULL;

CMAP_CONFIG_LOOP(BOOL_VAR, STRING_VAR, STRINGS_VAR)

/*******************************************************************************
*******************************************************************************/

static char config_is_true(const char * v)
{
  return ((v != NULL) && (!strcasecmp(v, "true") || !strcmp(v, "1")));
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_IMPL(name, ENV_VAR) \
void cmap_config_set_##name(char v) \
{ \
  name##_val = v; \
  name##_val_ok = (1 == 1); \
} \
 \
char cmap_config_is_##name() \
{ \
  if(!name##_val_ok) cmap_config_set_##name(cmap_cli_is_##name() || \
    config_is_true(getenv("CMAP_" #ENV_VAR))); \
  return name##_val; \
}

#define STRING_IMPL(name, ENV_VAR, dft) \
void cmap_config_set_##name(const char * v) \
{ \
  free(name##_val); \
  name##_val = (v == NULL) ? NULL : strdup(v); \
  name##_val_ok = (1 == 1); \
} \
 \
const char * cmap_config_##name() \
{ \
  if(!name##_val_ok) \
  { \
    const char * tmp = cmap_cli_##name(); \
    if(tmp != NULL) cmap_config_set_##name(tmp); \
    else if((tmp = getenv("CMAP_" #ENV_VAR)) != NULL) \
      cmap_config_set_##name(tmp); \
    else cmap_config_set_##name(dft); \
  } \
  return name##_val; \
}

#define STRINGS_IMPL(name, ENV_VAR) \
void cmap_config_add_##name(const char * v) \
{ \
  cmap_strings_add(&name##_val, v); \
} \
 \
CMAP_STRINGS * cmap_config_##name() \
{ \
  if(!name##_val_ok) \
  { \
    cmap_strings_add_all(&name##_val, cmap_cli_##name()); \
    const char * name##_env = getenv("CMAP_" #ENV_VAR); \
    if(name##_env != NULL) \
    { \
      CMAP_STRINGS * tmp = cmap_strings_split(name##_env); \
      cmap_strings_add_all(&name##_val, tmp); \
      cmap_strings_delete(&tmp); \
    } \
    name##_val_ok = (1 == 1); \
  } \
  return name##_val; \
}

CMAP_CONFIG_LOOP(BOOL_IMPL, STRING_IMPL, STRINGS_IMPL)

/*******************************************************************************
*******************************************************************************/

#define BOOL_CLEAN(name, ENV_VAR) name##_val = (1 == 0);

#define STRING_CLEAN(name, ENV_VAR, dft) free(name##_val); name##_val = NULL;

#define STRINGS_CLEAN(name, ENV_VAR) cmap_strings_delete(&name##_val);

void cmap_config_clean()
{
  CMAP_CONFIG_LOOP(BOOL_CLEAN, STRING_CLEAN, STRINGS_CLEAN)
}
