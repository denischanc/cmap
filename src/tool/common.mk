
AM_CFLAGS = \
  -I$(top_srcdir)/src/tool \
  -I$(top_srcdir)/src/tool/parser \
  -I$(top_srcdir)/src/tool/part \
  -I$(top_builddir)/src/tool/tpl/build-main \
  -I$(top_builddir)/src/tool/tpl/compile \
  -I$(top_builddir)/src/tool/tpl/prj-multiple \
  -I$(top_builddir)/src/tool/tpl/prj-simple \
  -DCMAP_BINDIR="\"$(bindir)\"" \
  -DCMAP_LIBDIR="\"$(libdir)\"" \
  -DCMAP_INCLUDEDIR="\"$(includedir)\"" \
  -Wall
