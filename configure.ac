#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([cmap], [0.1.0], [denis.chancogne@free.fr])
LT_INIT([win32-dll])
AC_CONFIG_SRCDIR([src/cmap.c])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_AWK
AC_PROG_YACC
AC_PROG_CC
AC_PROG_LEX([noyywrap])
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

# Checks for libraries.
AC_CHECK_LIB([uv], [uv_run])

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h libintl.h stdint.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strdup])

# Checks for platform.
AC_MSG_NOTICE([Host cpu : --$host_cpu--])
AC_MSG_NOTICE([Host vendor : --$host_vendor--])
AC_MSG_NOTICE([Host os : --$host_os--])

AM_CONDITIONAL([LINUX], [echo $host_os | grep -q linux])
AM_CONDITIONAL([CYGWIN], [echo $host_os | grep -q cygwin])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/components/Makefile
  src/components/console/Makefile
  src/components/scheduler/Makefile
  src/core/Makefile
  src/core/prototype/Makefile
  src/core/cmp/Makefile
  src/kernel/Makefile
  src/main/Makefile
  src/parser/Makefile
  src/util/Makefile
  test/Makefile
  test/kernel/Makefile
  test/mem/Makefile
  test/util/Makefile
])
AC_OUTPUT
