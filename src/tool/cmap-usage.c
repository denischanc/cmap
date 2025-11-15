
#include "cmap-usage.h"

#include <stdio.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

#define USAGE_OFF 2
#define USAGE_VAL 18
#define USAGE_DESC 60

/*******************************************************************************
*******************************************************************************/

static inline void print_sp(int nb)
{
  for(int j = 0; j < nb; j++) printf(" ");
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

static void print_desc(const char * desc)
{
  const char * cur = desc;
  int len = strlen(cur);
  while(1 == 1)
  {
    if(len <= USAGE_DESC) { printf("%s\n", cur); return; }
    else
    {
      printf("%s\n", sub_desc(cur));
      print_sp(USAGE_OFF + USAGE_VAL);

      cur += USAGE_DESC;
      len -= USAGE_DESC;
    }
  }
}

/*******************************************************************************
*******************************************************************************/

static void print_val_desc(const char * val, const char * desc)
{
  print_sp(USAGE_OFF);
  printf(val);

  int len = strlen(val);
  if(len >= USAGE_VAL)
  {
    printf("\n");
    print_sp(USAGE_OFF + USAGE_VAL);
  }
  else print_sp(USAGE_VAL - len);

  print_desc(desc);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_USAGE_PUBLIC cmap_usage_public = {print_val_desc};
