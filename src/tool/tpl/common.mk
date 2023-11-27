
CMAP_TPL_CFLAGS = \
  -DCMAP_INSTALL_BINDIR="\"$(bindir)\"" \
  -DCMAP_INSTALL_LIBDIR="\"$(libdir)\"" \
  -DCMAP_INSTALL_INCLUDEDIR="\"$(includedir)\""

cmap-%.h cmap-%.c:
	$(top_builddir)/src/tool/tpl/tpl2hc.sh $<
