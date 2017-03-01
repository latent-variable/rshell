#!/bin/sh

(ls);
sleep 1;
echo last && (echo first && echo second);
(ls && echo hey)
sleep 1
(echo a && echo b) && echo c 
sleep 1
(echo a && echo b) || (echo c && echo d ) #should only echo and b
sleep 1 && echo you get the message && ( sleep 1; echo nested && ( sleep 1; echo precedence 1 && ( sleep 1; echo precedence 2 && (sleep 1; echo precedence 3 ))))