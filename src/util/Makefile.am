
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-consumedtime.h \
  cmap-set-define.h \
  cmap-set.h \
  cmap-slist-define.h \
  cmap-slist.h \
  cmap-tree-define.h \
  cmap-tree.h \
  cmap-util.h

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  cmap-set.c \
  cmap-slist.c \
  cmap-tree.c \
  cmap-util.c
if CONSUMED_TIME
libutil_la_SOURCES += cmap-consumedtime.c
endif
libutil_la_CFLAGS = $(CMAP_CFLAGS)
