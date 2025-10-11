
include $(top_srcdir)/test/common.mk

CMAP_TOOL_FLAGS = -m

EXTRA_DIST = \
  test001.cmap \
  test002.cmap \
  test003.cmap \
  test004.cmap

noinst_PROGRAMS = \
  test001 \
  test002 \
  test003 \
  test004

AM_CFLAGS = $(CMAP_TEST_CFLAGS)
LDADD = $(CMAP_TEST_LDADD_COMMON)

nodist_test001_SOURCES = test001.c
nodist_test002_SOURCES = test002.c
nodist_test003_SOURCES = test003.c
nodist_test004_SOURCES = test004.c

CLEANFILES = \
  test001.c \
  test002.c \
  test003.c \
  test004.c
