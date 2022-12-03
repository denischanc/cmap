
include $(top_srcdir)/src/common.mk

include_cmap_HEADERS = \
  cmap-aisle-ext.h \
  cmap-mem-type.h \
  cmap-log-type.h \
  cmap-env-type.h \
  cmap-proc-ctx-type.h \
  cmap-kernel-type.h

noinst_HEADERS = \
  cmap-aisle.h \
  cmap-mem.h \
  cmap-mem-define.h \
  cmap-kernel.h \
  cmap-kernel-define.h \
  cmap-global-env.h \
  cmap-aislestore.h \
  cmap-pool.h \
  cmap-pool-handlers.h \
  cmap-log.h \
  cmap-log-define.h \
  cmap-env.h \
  cmap-proc-ctx.h

noinst_LTLIBRARIES = libkernel.la

libkernel_la_SOURCES = \
  cmap-mem.c \
  cmap-kernel.c \
  cmap-global-env.c \
  cmap-aislestore.c \
  cmap-pool.c \
  cmap-pool-handlers.c \
  cmap-aisle.c \
  cmap-log.c \
  cmap-env.c \
  cmap-proc-ctx.c
libkernel_la_CFLAGS = $(CMAP_CFLAGS)
