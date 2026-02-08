
#include "cmap-usage.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-config.h"
#include "cmap-console.h"
#include "cmap-tool.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

#define USAGE_OFF 2
#define USAGE_VAL 18
#define USAGE_DESC 60

/*******************************************************************************
*******************************************************************************/

static inline void display_sp(int nb)
{
  for(int j = 0; j < nb; j++) cmap_console_public.error(" ");
}

/*******************************************************************************
*******************************************************************************/

static const char * sub_desc(const char * desc)
{
  static char buffer[USAGE_DESC + 1];
  strncpy(buffer, desc, USAGE_DESC);
  buffer[USAGE_DESC] = 0;
  return buffer;
}

static void display_desc(const char * desc)
{
  const char * cur = desc;
  int len = strlen(cur);
  while(len > 0)
  {
    if(len <= USAGE_DESC) { cmap_console_public.error("%s\n", cur); len = 0; }
    else
    {
      cmap_console_public.error("%s\n", sub_desc(cur));
      display_sp(USAGE_OFF + USAGE_VAL);

      cur += USAGE_DESC;
      len -= USAGE_DESC;
    }
  }
}

/*******************************************************************************
*******************************************************************************/

static void display_val_desc(const char * val, const char * desc)
{
  display_sp(USAGE_OFF);
  cmap_console_public.error("%s%s%s", CMAP_ESC_YELLOW, val, CMAP_ESC_RST);

  int len = strlen(val);
  if(len >= USAGE_VAL)
  {
    cmap_console_public.error("\n");
    display_sp(USAGE_OFF + USAGE_VAL);
  }
  else display_sp(USAGE_VAL - len);

  display_desc(desc);
}

/*******************************************************************************
*******************************************************************************/

static void usage_opt(const char * long_opt, char opt, char is_arg,
  const char * desc)
{
  char * val = NULL;
  cmap_string_public.append_args(&val,
    "-%c,--%s%s", opt, long_opt, is_arg ? " [value]" : "");

  display_val_desc(val, desc);

  free(val);
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_USAGE(ID, name, long_opt, opt, env_var, desc) \
  if(id == ID) usage_opt(#long_opt, opt, (1 == 0), desc);

#define STRING_USAGE(ID, name, long_opt, opt, env_var, desc, dft) \
  if(id == ID) usage_opt(#long_opt, opt, (1 == 1), desc);

#define STRINGS_USAGE(ID, name, long_opt, opt, env_var, desc) \
  if(id == ID) usage_opt(#long_opt, opt, (1 == 1), desc);

static int usage(const char * desc, int * config_ids)
{
  cmap_console_public.error("%susage%s: %s ",
    CMAP_ESC_BBLUE, CMAP_ESC_RST, cmap_tool_name);

  cmap_console_public.error(desc, "(options)");
  cmap_console_public.error("\n%soptions%s:\n", CMAP_ESC_BBLUE, CMAP_ESC_RST);

  int id = CMAP_CONFIG_ID_HELP;
  CMAP_CONFIG_LOOP(BOOL_USAGE, STRING_USAGE, STRINGS_USAGE)
  while((id = *config_ids) != 0)
  {
    CMAP_CONFIG_LOOP(BOOL_USAGE, STRING_USAGE, STRINGS_USAGE)
    config_ids++;
  }

  return EXIT_FAILURE;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_USAGE_PUBLIC cmap_usage_public = {display_val_desc, usage};
