
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-set-define.h \
  cmap-set.h \
  cmap-stack-define.h \
  cmap-stack.h \
  cmap-tree-define.h \
  cmap-tree.h \
  cmap-util.h

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  cmap-set.c \
  cmap-stack.c \
  cmap-tree.c \
  cmap-util.c
libutil_la_CFLAGS = $(CMAP_CFLAGS)
