
CMAP_TPL_CFLAGS = \
  -DCMAP_INSTALL_BINDIR="\"$(bindir)\"" \
  -DCMAP_INSTALL_LIBDIR="\"$(libdir)\"" \
  -DCMAP_INSTALL_INCLUDEDIR="\"$(includedir)\""

cmap-prj-$(CMAP_PRJ_NATURE)-%.c: %.tpl
	$(top_builddir)/src/tool/tpl/tpl2hc.sh $< $@ C

cmap-prj-$(CMAP_PRJ_NATURE)-%.h: %.tpl
	$(top_builddir)/src/tool/tpl/tpl2hc.sh $< $@ INCLUDE
