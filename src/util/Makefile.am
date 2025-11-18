
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-consumedtime.h \
  cmap-util.h \
  cmap-uv.h

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  cmap-util.c \
  cmap-uv.c
if CONSUMED_TIME
libutil_la_SOURCES += cmap-consumedtime.c
endif
