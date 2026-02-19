
#include "cmap-usage.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-cli.h"
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
  for(int j = 0; j < nb; j++) cmap_console_error(" ");
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
    if(len <= USAGE_DESC) { cmap_console_error("%s\n", cur); len = 0; }
    else
    {
      cmap_console_error("%s\n", sub_desc(cur));
      display_sp(USAGE_OFF + USAGE_VAL);

      cur += USAGE_DESC;
      len -= USAGE_DESC;
    }
  }
}

/*******************************************************************************
*******************************************************************************/

void cmap_usage_display_val_desc(const char * val, const char * desc)
{
  display_sp(USAGE_OFF);
  cmap_console_error("%s%s%s", CMAP_ESC_YELLOW, val, CMAP_ESC_RST);

  int len = strlen(val);
  if(len >= USAGE_VAL)
  {
    cmap_console_error("\n");
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
  cmap_string_append_args(&val,
    "-%c,--%s%s", opt, long_opt, is_arg ? " [value]" : "");

  cmap_usage_display_val_desc(val, desc);

  free(val);
}

/*******************************************************************************
*******************************************************************************/

#define NO_ARG_USAGE(ID, name, long_opt, opt, desc) \
  if(id == CMAP_CLI_ID_##ID) usage_opt(#long_opt, opt, (1 == 0), desc);

#define ARG_USAGE(ID, name, long_opt, opt, desc) \
  if(id == CMAP_CLI_ID_##ID) usage_opt(#long_opt, opt, (1 == 1), desc);

int cmap_usage(const char * desc, int * config_ids)
{
  cmap_console_error("%susage%s: %s ",
    CMAP_ESC_BBLUE, CMAP_ESC_RST, cmap_tool_name);

  cmap_console_error(desc, "(options)");
  cmap_console_error("\n%soptions%s:\n", CMAP_ESC_BBLUE, CMAP_ESC_RST);

  int id = CMAP_CLI_ID_HELP;
  CMAP_CLI_LOOP(NO_ARG_USAGE, ARG_USAGE, ARG_USAGE)
  while((id = *config_ids) != 0)
  {
    CMAP_CLI_LOOP(NO_ARG_USAGE, ARG_USAGE, ARG_USAGE)
    config_ids++;
  }

  return EXIT_FAILURE;
}
