#!/bin/sh

echo  last && ( echo first && echo second)
(echo A && echo B) || (echo C && echo D)
(ls);
(ls && echo hey)
sleep 1
(echo a && echo b) && echo c 
sleep 1
(echo a && echo b) || (echo c && echo d ) #should only echo and