
include $(top_srcdir)/src/tool/common.mk

TPL2HC_SH = $(top_builddir)/src/tool/tpl/tpl2hc.sh

TPL_NATURE = $(notdir $(srcdir))

cmap-$(TPL_NATURE)-%.c: %.tpl $(TPL2HC_SH)
	$(TPL2HC_SH) $< $@ C

cmap-$(TPL_NATURE)-%.h: %.tpl $(TPL2HC_SH)
	$(TPL2HC_SH) $< $@ INCLUDE
