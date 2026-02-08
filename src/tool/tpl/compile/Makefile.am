
include $(top_srcdir)/src/tool/tpl/common.mk

BUILT_SOURCES = cmap-compile-bin.mk.h cmap-compile-module.mk.h

EXTRA_DIST = bin.mk.tpl module.mk.tpl

noinst_LTLIBRARIES = libcompile.la

nodist_libcompile_la_SOURCES = $(BUILT_SOURCES:.h=.c)

CLEANFILES = $(BUILT_SOURCES) $(BUILT_SOURCES:.h=.c)
