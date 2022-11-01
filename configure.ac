#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cmap], [0.1.0], [denis.chancogne@free.fr])
AC_CONFIG_SRCDIR([src/kernel/cmap-kernel.c])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([src/config.h])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
LT_INIT([win32-dll])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Checks for platform.
AC_MSG_NOTICE([Host cpu : --$host_cpu--])
AC_MSG_NOTICE([Host vendor : --$host_vendor--])
AC_MSG_NOTICE([Host os : --$host_os--])

## os
AM_CONDITIONAL([LINUX], [echo $host_os | grep -q linux])
AM_CONDITIONAL([CYGWIN], [echo $host_os | grep -q cygwin])

AC_CONFIG_FILES([ \
  Makefile \
  src/Makefile \
  src/include/cmap/Makefile \
  src/util/Makefile \
  src/core/Makefile \
  src/core/prototype/Makefile \
  src/kernel/Makefile \
  src/components/Makefile \
  src/components/console/Makefile \
  test/Makefile \
  test/util/Makefile \
  test/mem/Makefile \
  test/kernel/Makefile \
])
AC_OUTPUT
