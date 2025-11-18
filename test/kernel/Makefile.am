
include $(top_srcdir)/test/common.mk

SH_TEST_FILES = test003.sh test006.sh
CMAP_TEST_FILES = $(SH_TEST_FILES:.sh=.cmap)

noinst_PROGRAMS = \
  $(CMAP_TEST_FILES:.cmap=) \
  test001 \
  test002 \
  test004 \
  test005

test001_SOURCES = test001.c
test002_SOURCES = test002.c
nodist_test003_SOURCES = test003.c
test004_SOURCES = test004.c
test005_SOURCES = test005.c
nodist_test006_SOURCES = test006.c

DISTCLEANFILES = $(SH_TEST_FILES:.sh=.out) $(SH_TEST_FILES:.sh=.err)
