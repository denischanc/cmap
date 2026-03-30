
#include "cmap-op-handler-string.h"
#include "cmap-op-handler.h"

#include <stdio.h>
#include "cmap-string.h"
#include "cmap-int.h"
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

void cmap_op_handler_string_append(CMAP_STRING * dst, CMAP_MAP * src)
{
  unsigned char nature = CMAP_NATURE(src);
  if(nature == CMAP_STRING_NATURE)
  {
    char * val = cmap_string_val((CMAP_STRING *)src);
    cmap_string_append(dst, val);
  }
  else if(nature == CMAP_INT_NATURE)
  {
    char buffer[22]; /* 64 bits => 70 / 10 * 3 + 1 ("\n") = 22 */
    int64_t i = cmap_int_get((CMAP_INT *)src);
    snprintf(buffer, sizeof(buffer), "%ld", i);
    cmap_string_append(dst, buffer);
  }
  else
  {
    char buffer[19]; /* 64 bits => 64 / 8 * 2 + 2 ("0x") + 1 ("\n") = 19 */
    snprintf(buffer, sizeof(buffer), "%p", src);
    cmap_string_append(dst, buffer);
  }
}

/*******************************************************************************
*******************************************************************************/

static char op_add_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  if((map_dst == NULL) || (CMAP_NATURE(map_dst) != CMAP_STRING_NATURE))
    return (1 == 0);

  if(map_src == NULL) cmap_string_append((CMAP_STRING *)map_dst, "null");
  else cmap_op_handler_string_append((CMAP_STRING *)map_dst, map_src);
  return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

static char op_add(CMAP_MAP ** map_dst, CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  if((map_l == NULL) || (CMAP_NATURE(map_l) != CMAP_STRING_NATURE))
    return (1 == 0);

  CMAP_STRING * ret = CMAP_STRING("", 0, proc_ctx);
  cmap_op_handler_string_append(ret, map_l);
  cmap_op_handler_string_append(ret, map_r);
  *map_dst = (CMAP_MAP *)ret;
  return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_OP_HANDLER cmap_op_handler_string_add = {op_add};

const CMAP_OP_HANDLER_SELF cmap_op_handler_string_add_self = {op_add_self};
