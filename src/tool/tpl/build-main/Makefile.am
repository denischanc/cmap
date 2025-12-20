
include $(top_srcdir)/src/tool/tpl/common.mk

BUILT_SOURCES = cmap-build-main-main-fn.h cmap-build-main-main.h

EXTRA_DIST = main-fn.tpl main.tpl

noinst_LTLIBRARIES = libbuild-main.la

nodist_libbuild_main_la_SOURCES = $(BUILT_SOURCES:.h=.c)

CLEANFILES = $(BUILT_SOURCES) $(BUILT_SOURCES:.h=.c)
