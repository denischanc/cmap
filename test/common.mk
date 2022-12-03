
include $(top_srcdir)/src/common.mk

CMAP_TEST_SRC_PATH = $(top_srcdir)/test
CMAP_TEST_BUILD_PATH = $(top_builddir)/test

CMAP_TEST_CFLAGS = \
  $(CMAP_CFLAGS) \
  -I$(CMAP_TEST_SRC_PATH)/util

CMAP_TEST_LDADD = \
  $(CMAP_TEST_BUILD_PATH)/util/libtest.la \
  $(CMAP_BUILD_PATH)/libcmap.la
