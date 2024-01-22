
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-set-define.h \
  cmap-set.h \
  cmap-slist-define.h \
  cmap-slist.h \
  cmap-stree-define.h \
  cmap-stree.h

noinst_LTLIBRARIES = libscore.la

libscore_la_SOURCES = \
  cmap-set.c \
  cmap-slist.c \
  cmap-stree.c
libscore_la_CFLAGS = $(CMAP_CFLAGS)
