
include $(top_srcdir)/src/common.mk

CMAP_TEST_SRC_PATH = $(top_srcdir)/test
CMAP_TEST_BUILD_PATH = $(top_builddir)/test

EXTRA_DIST = $(SH_TEST_FILES) $(CMAP_TEST_FILES)

AM_CFLAGS += -I$(CMAP_TEST_SRC_PATH)/util

LDADD = \
  $(CMAP_TEST_BUILD_PATH)/util/libtest.la \
  $(CMAP_BUILD_PATH)/libcmap.la

%.c: $(srcdir)/%.cmap $(CMAP_TOOL)
	$(CMAP_TOOL) build $< $(@:.c=) --relative-inc --only-c $(CMAP_TOOL_FLAGS)

CLEANFILES = $(CMAP_TEST_FILES:.cmap=.c)
