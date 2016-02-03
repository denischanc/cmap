#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cmap], [0.1.0], [denis.chancogne@free.fr])
AC_CONFIG_SRCDIR([src/kernel/core/mem/cmap-mem.h])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
LT_INIT([win32-dll])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([ \
  Makefile \
  src/Makefile \
  src/util/Makefile \
  src/kernel/Makefile \
  src/kernel/core/Makefile \
  src/kernel/core/mem/Makefile \
  src/kernel/core/map/Makefile \
  test/Makefile \
  test/util/Makefile \
  test/mem/Makefile \
  test/kernel/Makefile \
])
AC_OUTPUT
