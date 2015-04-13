EXTRA_DIST=wordlist.txt
lib_LTLIBRARIES=libclarity.la
libclarity_la_SOURCES=libclarity.cc
libclarity_la_CPPFLAGS=-I.. -DPKGDATADIR="\"$(pkgdatadir)\""
check_PROGRAMS=checkclarity
checkclarity_SOURCES=checkclarity.cc
checkclarity_LDADD=libclarity.la
include_HEADERS=clarity.h
pkgdata_DATA=wordlist.txt
