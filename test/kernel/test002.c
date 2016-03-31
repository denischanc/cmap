/*******************************************************************************
@TEST_DESC@ Add/rm maps in list
*******************************************************************************/

#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-fw.h"

#include "test-assert.h"
#include <stdlib.h>

#define NB_ELMTS_1 3

int main(int argc, char * argv[])
{
  cmap_kernel_create(NULL);

  CMAP_LIST * list = _L_(3);

  CMAP_MAP elmts[NB_ELMTS_1] = {0};
  int i = 0;

  for(i = 0; i < NB_ELMTS_1; i++) CMAP_CALL_ARGS(list, push, elmts + i);
  for(i = 0; i < NB_ELMTS_1; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, unshift) == elmts + i);

  for(i = 0; i < NB_ELMTS_1; i++) CMAP_CALL_ARGS(list, shift, elmts + i);
  for(i = 0; i < NB_ELMTS_1; i++)
    CMAP_TEST_ASSERT_NOMSG(CMAP_CALL(list, pop) == elmts + i);

  return cmap_kernel() -> main(argc, argv);
}
