
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src test

wc:
	wc -l $$(find $(srcdir)/src $(srcdir)/test \
		-name "*.c" -o -name "*.h" -o -name "*.cmap")

test_desc:
	$(srcdir)/test/test_desc.sh

.PHONY: wc test_desc
