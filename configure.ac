#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(furlong, 1.0.0 furlong@monketree.org)
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
CC_CHECK_CFLAGS_APPEND([-fvisibility=hidden])
CC_CHECK_CFLAGS_APPEND([-g])
CC_CHECK_CFLAGS_APPEND([-std=gnu89])
CC_CHECK_CFLAGS_APPEND([-pedantic])
CC_CHECK_CFLAGS_APPEND([-Wall])
CC_CHECK_CFLAGS_APPEND([-Wextra])
CC_CHECK_CFLAGS_APPEND([-Wno-unused-parameter])
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h netdb.h stdlib.h string.h sys/socket.h unistd.h])

AC_CHECK_HEADERS([unistd.h stdint.h sys/types.h sys/sendfile.h sys/signal.h])
AC_CHECK_HEADERS([ctype.h sys/resource.h sys/socket.h sys/epoll.h fcntl.h netdb.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SSIZE_T

# Checks for library functions.



AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects] )


AC_CHECK_FUNCS([memset socket])

AC_OUTPUT(Makefile src/Makefile include/Makefile doc/Makefile examples/Makefile man/Makefile scripts/Makefile)
