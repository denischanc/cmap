
CMAP_SRC_PATH = $(top_srcdir)/src
CMAP_INCLUDE_PATH = $(CMAP_SRC_PATH)/include
CMAP_KERNEL_PATH = $(CMAP_SRC_PATH)/kernel

CMAP_BUILD_PATH = $(top_builddir)/src

CMAP_KERNEL_CFLAGS = \
  -I$(CMAP_INCLUDE_PATH) \
  -I$(CMAP_SRC_PATH) \
  -I$(CMAP_SRC_PATH)/util \
  -I$(CMAP_SRC_PATH)/core \
  -I$(CMAP_KERNEL_PATH) \
  -I$(CMAP_KERNEL_PATH)/prototype \
  -I$(CMAP_KERNEL_PATH)/console \
  -Wall
