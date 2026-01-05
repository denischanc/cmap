
SUBDIRS = util mem parser slist kernel

EXTRA_DIST = setenv.sh

TESTS =
XFAIL_TESTS =
include mem/tests.mk
include parser/tests.mk
include slist/tests.mk
include kernel/tests.mk

AM_TESTS_ENVIRONMENT = . $(srcdir)/setenv.sh;
