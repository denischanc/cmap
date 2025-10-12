
include $(top_srcdir)/test/common.mk

CMAP_TOOL_FLAGS = -m

EXTRA_DIST = \
  test001.cmap \
  test002.cmap \
  test003.cmap \
  test004.cmap \
  test005.cmap \
  test006.cmap

noinst_PROGRAMS = $(EXTRA_DIST:.cmap=)

AM_CFLAGS = $(CMAP_TEST_CFLAGS)
LDADD = $(CMAP_TEST_LDADD_COMMON)

nodist_test001_SOURCES = test001.c
nodist_test002_SOURCES = test002.c
nodist_test003_SOURCES = test003.c
nodist_test004_SOURCES = test004.c
nodist_test005_SOURCES = test005.c
nodist_test006_SOURCES = test006.c

CLEANFILES = $(EXTRA_DIST:.cmap=.c)
