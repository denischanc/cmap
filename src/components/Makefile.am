
include $(top_srcdir)/src/common.mk

SUBDIRS = console scheduler

noinst_HEADERS = cmap-cmap.h

noinst_LTLIBRARIES = libcomponents.la

libcomponents_la_SOURCES = cmap-cmap.c
libcomponents_la_CFLAGS = $(CMAP_CFLAGS)
libcomponents_la_LIBADD = \
  console/libconsole.la \
  scheduler/libscheduler.la
