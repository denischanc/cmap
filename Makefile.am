
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src test

.PHONY: wc

wc:
	wc -l $$(find $(srcdir)/src $(srcdir)/test \
		-name "*.c" -o -name "*.h" -o -name "*.cmap")
