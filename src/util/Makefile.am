
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-consumedtime.h \
  cmap-util.h

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  cmap-util.c
if CONSUMED_TIME
libutil_la_SOURCES += cmap-consumedtime.c
endif
