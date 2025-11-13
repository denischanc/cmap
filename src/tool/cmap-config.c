
#include "cmap-config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

#define USAGE_OFF 2
#define USAGE_OPTS 18
#define USAGE_DESC 60

/*******************************************************************************
*******************************************************************************/

#define BOOL_VAR(ID, name, long_opt, opt, env_var, desc) \
  static char name##_proc_ok = (1 == 0), name##_proc, name##_opt = (1 == 0);

#define STRING_VAR(ID, name, long_opt, opt, env_var, desc, dft) \
  static char * name##_proc = NULL, * name##_opt = NULL; \
  static const char * name##_dft = dft;

#define STRINGS_VAR(ID, name, long_opt, opt, env_var, desc) \
  static CMAP_STRINGS * name##_all = NULL; \
  static char name##_env_ok = (1 == 0);

CMAP_CONFIG_LOOP(BOOL_VAR, STRING_VAR, STRINGS_VAR)

/*******************************************************************************
*******************************************************************************/

static char config_is_true(const char * v)
{
  char * diff = strdup(v), * i;
  for(i = diff; *i != 0; i++) if((*i >= 'A') && (*i <= 'Z')) *i += 'a' - 'A';

  int cmp = strcmp(diff, "true");
  free(diff);
  return !cmp;
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_IMPL(ID, name, long_opt, opt, env_var, desc) \
static void set_##name(char v) \
{ \
  name##_proc_ok = (1 == 1); \
  name##_proc = v; \
} \
 \
static char is_##name() \
{ \
  if(name##_proc_ok) return name##_proc; \
  if(name##_opt) return (1 == 1); \
  const char * name##_env = getenv(#env_var); \
  if(name##_env != NULL) return config_is_true(name##_env); \
  return (1 == 0); \
}

#define STRING_IMPL(ID, name, long_opt, opt, env_var, desc, dft) \
static void set_##name(const char * v) \
{ \
  free(name##_proc); \
  name##_proc = strdup(v); \
} \
 \
static const char * name() \
{ \
  if(name##_proc != NULL) return name##_proc; \
  if(name##_opt != NULL) return name##_opt; \
  const char * name##_env = getenv(#env_var); \
  if(name##_env != NULL) return name##_env; \
  return name##_dft; \
}

#define STRINGS_IMPL(ID, name, long_opt, opt, env_var, desc) \
static void add_##name(const char * v) \
{ \
  cmap_strings_public.add(&name##_all, v); \
} \
 \
static CMAP_STRINGS * name() \
{ \
  if(!name##_env_ok) \
  { \
    name##_env_ok = (1 == 1); \
    const char * name##_env = getenv(#env_var); \
    if(name##_env != NULL) \
    { \
      CMAP_STRINGS * tmp = cmap_strings_public.split(name##_env); \
      cmap_strings_public.add_all(&name##_all, tmp); \
      cmap_strings_public.delete(&tmp); \
    } \
  } \
  return name##_all; \
}

CMAP_CONFIG_LOOP(BOOL_IMPL, STRING_IMPL, STRINGS_IMPL)

/*******************************************************************************
*******************************************************************************/

#define BOOL_NB(ID, name, long_opt, opt, env_var, desc) nb++;
#define STRING_NB(ID, name, long_opt, opt, env_var, desc, dft) nb++;
#define STRINGS_NB(ID, name, long_opt, opt, env_var, desc) nb++;

static inline int nb_opts()
{
  int nb = 0;
  CMAP_CONFIG_LOOP(BOOL_NB, STRING_NB, STRINGS_NB)
  return nb;
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_OPTS(ID, name, long_opt, opt, env_var, desc) \
  struct option opt_##name = {#long_opt, no_argument, NULL, opt}; \
  long_opts[i++] = opt_##name; \
  cmap_string_public.append_args(&short_opts, "%c", opt);

#define BOOL_CASE(ID, name, long_opt, opt, env_var, desc) \
  case opt: name##_opt = (1 == 1); break;

#define STRING_OPTS(ID, name, long_opt, opt, env_var, desc, dft) \
  struct option opt_##name = {#long_opt, required_argument, NULL, opt}; \
  long_opts[i++] = opt_##name; \
  cmap_string_public.append_args(&short_opts, "%c:", opt);

#define STRING_CASE(ID, name, long_opt, opt, env_var, desc, dft) \
  case opt: free(name##_opt); name##_opt = strdup(optarg); break;

#define STRINGS_OPTS(ID, name, long_opt, opt, env_var, desc) \
  struct option opt_##name = {#long_opt, required_argument, NULL, opt}; \
  long_opts[i++] = opt_##name; \
  cmap_string_public.append_args(&short_opts, "%c:", opt);

#define STRINGS_CASE(ID, name, long_opt, opt, env_var, desc) \
  case opt: cmap_strings_public.add(&name##_all, optarg); break;

static void mng_opts(int * argc, char *** argv)
{
  struct option last_opt = {NULL, 0, NULL, 0},
    * long_opts = malloc((nb_opts() + 1) * sizeof(struct option));
  char * short_opts = NULL;

  int i = 0;
  CMAP_CONFIG_LOOP(BOOL_OPTS, STRING_OPTS, STRINGS_OPTS)
  long_opts[i] = last_opt;

  optind = 1;
  int o;
  while((o = getopt_long(*argc, *argv, short_opts, long_opts, NULL)) != -1)
  {
    switch(o)
    {
      CMAP_CONFIG_LOOP(BOOL_CASE, STRING_CASE, STRINGS_CASE)
    }
  }

  free(long_opts);
  free(short_opts);

  *argc -= optind;
  *argv += optind;
}

/*******************************************************************************
*******************************************************************************/

static inline void usage_opt_sp(int nb)
{
  for(int j = 0; j < nb; j++) printf(" ");
}

static const char * usage_opt_sub_desc(const char * desc)
{
  static char buffer[USAGE_DESC + 1];
  strncpy(buffer, desc, USAGE_DESC);
  buffer[USAGE_DESC] = 0;
  return buffer;
}

static void usage_opt_desc(const char * desc)
{
  const char * cur = desc;
  int len = strlen(cur);
  while(1 == 1)
  {
    if(len <= USAGE_DESC) { printf("%s\n", cur); return; }
    else
    {
      printf("%s\n", usage_opt_sub_desc(cur));
      usage_opt_sp(USAGE_OFF + USAGE_OPTS);

      cur += USAGE_DESC;
      len -= USAGE_DESC;
    }
  }
}

static void usage_opt(const char * long_opt, char opt, char is_arg,
  const char * desc)
{
  usage_opt_sp(USAGE_OFF);

  char * line = NULL;
  cmap_string_public.append_args(&line, "-%c,--%s", opt, long_opt);
  if(is_arg) cmap_string_public.append(&line, " [value]");
  int len = strlen(line);
  printf(line);
  free(line);

  if(len >= USAGE_OPTS)
  {
    printf("\n");
    usage_opt_sp(USAGE_OFF + USAGE_OPTS);
  }
  else usage_opt_sp(USAGE_OPTS - len);

  usage_opt_desc(desc);
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_USAGE(ID, name, long_opt, opt, env_var, desc) \
  if(id == ID) usage_opt(#long_opt, opt, (1 == 0), desc);

#define STRING_USAGE(ID, name, long_opt, opt, env_var, desc, dft) \
  if(id == ID) usage_opt(#long_opt, opt, (1 == 1), desc);

#define STRINGS_USAGE(ID, name, long_opt, opt, env_var, desc) \
  if(id == ID) usage_opt(#long_opt, opt, (1 == 1), desc);

static void usage(const char * this_name, const char * usage_ext,
  int * config_ids)
{
  printf("usage: %s %s (options)\noptions:\n", this_name, usage_ext);

  for(int i = 0; config_ids[i] != 0; i++)
  {
    int id = config_ids[i];
    CMAP_CONFIG_LOOP(BOOL_USAGE, STRING_USAGE, STRINGS_USAGE)
  }
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_CLEAN(ID, name, long_opt, opt, env_var, desc) \
  name##_proc_ok = (1 == 0); \
  name##_opt = (1 == 0);

#define STRING_CLEAN(ID, name, long_opt, opt, env_var, desc, dft) \
  free(name##_proc); name##_proc = NULL; \
  free(name##_opt); name##_opt = NULL;

#define STRINGS_CLEAN(ID, name, long_opt, opt, env_var, desc) \
  cmap_strings_public.delete(&name##_all); \
  name##_env_ok = (1 == 0);

static void clean()
{
  CMAP_CONFIG_LOOP(BOOL_CLEAN, STRING_CLEAN, STRINGS_CLEAN)
}

/*******************************************************************************
*******************************************************************************/

static void init_n_check(int * argc, char *** argv, int argc_min,
  const char * usage_ext, int * config_ids)
{
  const char * this_name = **argv;

  mng_opts(argc, argv);

  if(*argc < argc_min)
  {
    usage(this_name, usage_ext, config_ids);
    clean();
    exit(EXIT_FAILURE);
  }
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_SET(ID, name, long_opt, opt, env_var, desc) set_##name, is_##name,

#define STRING_SET(ID, name, long_opt, opt, env_var, desc, dft) \
  set_##name, name,

#define STRINGS_SET(ID, name, long_opt, opt, env_var, desc) add_##name, name,

const CMAP_CONFIG_PUBLIC cmap_config_public =
{
  CMAP_CONFIG_LOOP(BOOL_SET, STRING_SET, STRINGS_SET)
  clean, init_n_check
};
