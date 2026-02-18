#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([cmap], [0.1.0-dev], [denis.chancogne@free.fr])
LT_INIT([win32-dll])
AC_CONFIG_SRCDIR([src/cmap.c])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
### AC_PROG_AR
AC_PROG_AWK
AC_PROG_YACC
AC_PROG_CC
AC_PROG_LEX([noyywrap])
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_SED

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h strings.h sys/param.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday mkdir realpath setenv strcasecmp strchr strdup strerror strrchr])

# Checks for platform.
AC_MSG_NOTICE([Host cpu : --$host_cpu--])
AC_MSG_NOTICE([Host vendor : --$host_vendor--])
AC_MSG_NOTICE([Host os : --$host_os--])

AM_CONDITIONAL([CYGWIN], [echo $host_os | grep -q cygwin])

# Checks for compilation directives.
AC_ARG_ENABLE([consumed-time],
  [AS_HELP_STRING([--enable-consumed-time], [Consumed time logging.])], [], [])
AM_CONDITIONAL([CONSUMED_TIME], [test "x$enable_consumed_time" = "xyes"])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/components/Makefile
  src/components/cli/Makefile
  src/components/console/Makefile
  src/components/module/Makefile
  src/components/scheduler/Makefile
  src/core/Makefile
  src/core/cmp/Makefile
  src/core/op/Makefile
  src/core/prototype/Makefile
  src/core/score/Makefile
  src/kernel/Makefile
  src/loop/Makefile
  src/tool/Makefile
  src/tool/parser/Makefile
  src/tool/part/Makefile
  src/tool/tpl/Makefile
  src/tool/tpl/build-main/Makefile
  src/tool/tpl/compile/Makefile
  src/tool/tpl/prj-multiple/Makefile
  src/tool/tpl/prj-simple/Makefile
  src/tool/tpl/tpl2hc.sh
  src/util/Makefile
  test/Makefile
  test/kernel/Makefile
  test/mem/Makefile
  test/parser/Makefile
  test/slist/Makefile
  test/util/Makefile
])
AC_CONFIG_COMMANDS([tpl2hc.sh], [chmod +x src/tool/tpl/tpl2hc.sh])
AC_OUTPUT
