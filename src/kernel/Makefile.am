
include $(top_srcdir)/src/common.mk

SUBDIRS = core prototype console

include_HEADERS = \
  cmap-kernel.h \
  cmap-warehouse.h \
  cmap-pool.h \
  cmap-aisle.h \
  cmap-log.h

noinst_HEADERS = \
  cmap-global-env.h

noinst_LTLIBRARIES = libkernel.la

libkernel_la_SOURCES = \
  cmap-kernel.c \
  cmap-global-env.c \
  cmap-warehouse.c \
  cmap-pool.c \
  cmap-aisle.c \
  cmap-log.c
libkernel_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
libkernel_la_LIBADD = \
  core/libcore.la \
  prototype/libprototype.la \
  console/libconsole.la
