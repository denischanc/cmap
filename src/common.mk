
CMAP_SRC_PATH = $(top_srcdir)/src
CMAP_INCLUDE_PATH = $(CMAP_SRC_PATH)/include

CMAP_BUILD_PATH = $(top_builddir)/src

CMAP_KERNEL_CFLAGS = \
  -I$(CMAP_INCLUDE_PATH) \
  -I$(CMAP_SRC_PATH) \
  -I$(CMAP_SRC_PATH)/util \
  -I$(CMAP_SRC_PATH)/core \
  -I$(CMAP_SRC_PATH)/core/prototype \
  -I$(CMAP_SRC_PATH)/kernel \
  -I$(CMAP_SRC_PATH)/components \
  -I$(CMAP_SRC_PATH)/components/console \
  -I$(CMAP_SRC_PATH)/components/scheduler \
  -Wall
