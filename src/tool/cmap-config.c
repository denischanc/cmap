
#include "cmap-config.h"

#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "cmap-cli.h"

/*******************************************************************************
*******************************************************************************/

#define BOOL_VAR(name, env_var) \
  static char name##_val_ok = (1 == 0), name##_val = (1 == 0);

#define STRING_VAR(name, env_var, dft) \
  static char name##_val_ok = (1 == 0), * name##_val = NULL; \
  static const char * name##_dft = dft;

#define STRINGS_VAR(name, env_var) \
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

#define BOOL_IMPL(name, env_var) \
static void set_##name(char v) \
{ \
  name##_val = v; \
  name##_val_ok = (1 == 1); \
} \
 \
static char is_##name() \
{ \
  if(!name##_val_ok) set_##name(cmap_cli_public.is_##name() || \
    config_is_true(getenv(#env_var))); \
  return name##_val; \
}

#define STRING_IMPL(name, env_var, dft) \
static void set_##name(const char * v) \
{ \
  free(name##_val); \
  name##_val = (v == NULL) ? NULL : strdup(v); \
  name##_val_ok = (1 == 1); \
} \
 \
static const char * name() \
{ \
  if(!name##_val_ok) \
  { \
    const char * tmp = cmap_cli_public.name(); \
    if(tmp != NULL) set_##name(tmp); \
    else if((tmp = getenv(#env_var)) != NULL) set_##name(tmp); \
    else set_##name(name##_dft); \
  } \
  return name##_val; \
}

#define STRINGS_IMPL(name, env_var) \
static void add_##name(const char * v) \
{ \
  cmap_strings_public.add(&name##_val, v); \
} \
 \
static CMAP_STRINGS * name() \
{ \
  if(!name##_val_ok) \
  { \
    cmap_strings_public.add_all(&name##_val, cmap_cli_public.name()); \
    const char * name##_env = getenv(#env_var); \
    if(name##_env != NULL) \
    { \
      CMAP_STRINGS * tmp = cmap_strings_public.split(name##_env); \
      cmap_strings_public.add_all(&name##_val, tmp); \
      cmap_strings_public.delete(&tmp); \
    } \
    name##_val_ok = (1 == 1); \
  } \
  return name##_val; \
}

CMAP_CONFIG_LOOP(BOOL_IMPL, STRING_IMPL, STRINGS_IMPL)

/*******************************************************************************
*******************************************************************************/

#define BOOL_CLEAN(name, env_var) name##_val = (1 == 0);

#define STRING_CLEAN(name, env_var, dft) free(name##_val); name##_val = NULL;

#define STRINGS_CLEAN(name, env_var) cmap_strings_public.delete(&name##_val);

static void clean()
{
  CMAP_CONFIG_LOOP(BOOL_CLEAN, STRING_CLEAN, STRINGS_CLEAN)
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_SET(name, env_var) set_##name, is_##name,

#define STRING_SET(name, env_var, dft) set_##name, name,

#define STRINGS_SET(name, env_var) add_##name, name,

const CMAP_CONFIG_PUBLIC cmap_config_public =
{
  CMAP_CONFIG_LOOP(BOOL_SET, STRING_SET, STRINGS_SET)
  clean
};
