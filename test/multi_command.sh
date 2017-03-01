#!/bin/sh

ls && ls -l && sleep 1|| echo hello && sleep 1 || echo world && git status;



echo hello && sleep 1 && echo a && sleep 1; echo b &&echo c || echo d

echo a && sleep 1 &&  echo b && sleep 1; echo c && echo d || echo e # should not do this# && echo e
