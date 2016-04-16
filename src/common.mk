
CMAP_SRC_PATH = $(top_srcdir)/src
CMAP_KERNEL_SRC_PATH = $(CMAP_SRC_PATH)/kernel

CMAP_BUILD_PATH = $(top_builddir)/src

CMAP_KERNEL_CFLAGS = \
  -I$(CMAP_SRC_PATH) \
  -I$(CMAP_SRC_PATH)/util \
  -I$(CMAP_KERNEL_SRC_PATH) \
  -I$(CMAP_KERNEL_SRC_PATH)/core \
  -I$(CMAP_KERNEL_SRC_PATH)/console
