
include $(top_srcdir)/src/kernel/common.mk

CMAP_TEST_SRC_PATH = $(top_srcdir)/test
CMAP_TEST_BUILD_PATH = $(top_builddir)/test

CMAP_TEST_CFLAGS = \
  -I$(CMAP_TEST_SRC_PATH)/util
