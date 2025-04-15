
CMAP_TPL_CFLAGS = \
  -DCMAP_INSTALL_BINDIR="\"$(bindir)\"" \
  -DCMAP_INSTALL_LIBDIR="\"$(libdir)\"" \
  -DCMAP_INSTALL_INCLUDEDIR="\"$(includedir)\""

TPL2HC_SH = $(top_builddir)/src/tool/tpl/tpl2hc.sh

cmap-prj-$(CMAP_PRJ_NATURE)-%.c: %.tpl $(TPL2HC_SH)
	$(TPL2HC_SH) $< $@ C

cmap-prj-$(CMAP_PRJ_NATURE)-%.h: %.tpl $(TPL2HC_SH)
	$(TPL2HC_SH) $< $@ INCLUDE
