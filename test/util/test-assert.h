#ifndef __TEST_ASSERT_H__
#define __TEST_ASSERT_H__

#define CMAP_TEST_ASSERT(cond, msg) \
  cmap_test_assert(cond, __FILE__, __LINE__, msg)

#define CMAP_TEST_ASSERT_PTR(ptr) \
  CMAP_TEST_ASSERT(ptr != NULL, #ptr " ptr is not NULL")

#define CMAP_TEST_ASSERT_NOMSG(cond) \
  CMAP_TEST_ASSERT(cond, #cond)

void cmap_test_assert(char cond, const char * file, int line,
  const char * msg);

#endif
