
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-tree-define.h \
  cmap-tree.h \
  cmap-util.h

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  cmap-tree.c \
  cmap-util.c
libutil_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
