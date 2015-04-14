AC_INIT([Clarity], [1.1], [Robert J. Hansen <rjh@sixdemonbag.org>], [clarity], [http://rjhansen.github.com/clarity/])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/libclarity.cc])
AC_CONFIG_HEADERS([config.h])
AC_CHECK_HEADERS
AC_PREREQ([2.58])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
LT_INIT
dnl AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_14(noext,mandatory)
AC_OUTPUT([Makefile src/Makefile])
