#!/bin/sh
cd ..


bin/rshell 

ls && echo a ||exit && ls -l && ls -a || exit; #exit # || ls && exit;

