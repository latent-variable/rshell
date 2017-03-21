#!/bin/sh

ls #this should be executed while runnig bin/rshell
echo #this should be executed while runnig bin/rshell #; echo continue
cal #hello
ls -l && echo echo this #&& echo this not
(ls # && echo nothing# )
[ -e src # test -f src/main.cpp #]
(test src || #exit # echo hello ) && (cal && pwd )
