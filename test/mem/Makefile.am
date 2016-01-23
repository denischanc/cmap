
include $(top_srcdir)/test/common.mk

noinst_PROGRAMS = test001

test001_SOURCES = test001.c
test001_CFLAGS = $(CMAP_KERNEL_CFLAGS) $(CMAP_TEST_CFLAGS)
test001_LDADD = \
  $(CMAP_BUILD_PATH)/kernel/core/mem/libmem.la \
  $(CMAP_TEST_BUILD_PATH)/util/libtest.la

TESTS = test001
