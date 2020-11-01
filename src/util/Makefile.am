
include $(top_srcdir)/src/common.mk

include_HEADERS = \
  cmap-tree.h \
  cmap-tree-define.h \
  cmap-tree-internal.h \
  cmap-util-string.h \
  cmap-util-list.h \
  cmap-util-pool.h

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  cmap-tree.c \
  cmap-util-string.c \
  cmap-util-list.c \
  cmap-util-pool.c
libutil_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
