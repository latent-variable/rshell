#!/bin/sh
cd ..

bin/rshell
sleep 2
ls && echo a ||exit && ls -l && ls -a || exit; #exit # || ls && exit;

