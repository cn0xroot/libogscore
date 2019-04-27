#!/bin/sh

case $1 in
    './core/testcore') $@ -x sctp-test
    ;;
    *) $@
    ;;
esac
exit $?

