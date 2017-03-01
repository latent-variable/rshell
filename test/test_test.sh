#!/bin/sh


test -f test/exit.sh && echo test -f test/exit.sh true
sleep 1
[ -f test/exit.sh ] && echo [ -f test/exit.sh  ] true
sleep 1
test -d test && test -d src && echo test -d test && test -d true
sleep 1
[ -d test ] || [test -d src ] && (test src && echo checked in precedence)