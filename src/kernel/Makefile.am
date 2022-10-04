
include $(top_srcdir)/src/common.mk

SUBDIRS = prototype console

include_HEADERS = \
  cmap-aisle-external.h \
  cmap-kernel-external.h

noinst_HEADERS = \
  cmap-aisle.h \
  cmap-global-env.h \
  cmap-kernel-define.h \
  cmap-kernel.h \
  cmap-log.h \
  cmap-pool.h \
  cmap-pool-handlers.h \
  cmap-warehouse.h

noinst_LTLIBRARIES = libkernel.la

libkernel_la_SOURCES = \
  cmap-kernel.c \
  cmap-global-env.c \
  cmap-warehouse.c \
  cmap-pool.c \
  cmap-pool-handlers.c \
  cmap-pool-handlers.h \
  cmap-aisle.c \
  cmap-log.c
libkernel_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
libkernel_la_LIBADD = \
  prototype/libprototype.la \
  console/libconsole.la
