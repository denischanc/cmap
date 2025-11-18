
include_cmapdir = $(includedir)/cmap

CMAP_SRC_PATH = $(top_srcdir)/src
CMAP_BUILD_PATH = $(top_builddir)/src

AM_CFLAGS = \
  -I$(CMAP_SRC_PATH)/components/cli \
  -I$(CMAP_SRC_PATH)/components/console \
  -I$(CMAP_SRC_PATH)/components/scheduler \
  -I$(CMAP_SRC_PATH)/components \
  -I$(CMAP_SRC_PATH)/core/cmp \
  -I$(CMAP_SRC_PATH)/core/prototype \
  -I$(CMAP_SRC_PATH)/core/score \
  -I$(CMAP_SRC_PATH)/core \
  -I$(CMAP_SRC_PATH)/kernel \
  -I$(CMAP_SRC_PATH)/util \
  -I$(CMAP_SRC_PATH) \
  -I$(CMAP_BUILD_PATH)/components/console \
  -I$(CMAP_BUILD_PATH)/components/scheduler \
  -I$(CMAP_BUILD_PATH)/components \
  -Wall

if CONSUMED_TIME
AM_CFLAGS += -DCONSUMED_TIME
endif

CMAP_TOOL = $(CMAP_BUILD_PATH)/tool/cmap-tool
