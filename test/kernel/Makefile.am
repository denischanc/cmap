
include $(top_srcdir)/test/common.mk

EXTRA_DIST = test003.cmap test006.cmap

noinst_PROGRAMS = \
  test001 \
  test002 \
  test003 \
  test004 \
  test005 \
  test006

AM_CFLAGS = $(CMAP_TEST_CFLAGS)
LDADD = $(CMAP_TEST_LDADD_COMMON)

test001_SOURCES = test001.c
test002_SOURCES = test002.c
nodist_test003_SOURCES = test003.c
test004_SOURCES = test004.c
test005_SOURCES = test005.c
nodist_test006_SOURCES = test006.c

CLEANFILES = test003.c test006.c
