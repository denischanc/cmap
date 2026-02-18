/*******************************************************************************
@TEST_DESC@ Alloc/dealloc with 1024 memory
*******************************************************************************/

#include <stdlib.h>
#include "test-assert.h"
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-config.h"

#define ASSERT_NB_FREE(nb) \
  CMAP_TEST_ASSERT(cmap_mem_state() -> nb_block_free == nb, \
    "Check free block number")

#define ASSERT_NB_CHUNK(nb) \
  CMAP_TEST_ASSERT(cmap_mem_state() -> nb_chunk == nb, "Check chunk number")

int main(int argc, char * argv[])
{
  cmap_config_set_mem_chunk_size(1 << 10);
  CMAP_TEST_ASSERT(cmap_mem_is_this(), "This is cmap mem");

  ASSERT_NB_CHUNK(0);

  void * ptr1 = cmap_mem_alloc(100);
  CMAP_TEST_ASSERT_PTR(ptr1);
  ASSERT_NB_CHUNK(1);

  void * ptr2 = cmap_mem_alloc(300);
  CMAP_TEST_ASSERT_PTR(ptr2);
  ASSERT_NB_CHUNK(1);

  void * ptr3 = cmap_mem_alloc(10000);
  CMAP_TEST_ASSERT_PTR(ptr3);
  ASSERT_NB_CHUNK(2);

  cmap_mem_free(ptr1);

  void * ptr4 = cmap_mem_alloc(200);
  CMAP_TEST_ASSERT_PTR(ptr4);
  CMAP_TEST_ASSERT(ptr4 > ptr2, "Check ptr4");

  void * ptr5 = cmap_mem_alloc(100);
  CMAP_TEST_ASSERT_PTR(ptr5);
  CMAP_TEST_ASSERT(ptr5 == ptr1, "Check ptr5");

  ASSERT_NB_FREE(1);
  cmap_mem_free(ptr2);
  ASSERT_NB_FREE(2);
  cmap_mem_free(ptr3);
  ASSERT_NB_FREE(3);
  cmap_mem_free(ptr4);
  ASSERT_NB_FREE(2);
  cmap_mem_free(ptr5);
  ASSERT_NB_FREE(2);
  ASSERT_NB_CHUNK(2);

  return EXIT_SUCCESS;
}
