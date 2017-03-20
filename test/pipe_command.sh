#!/bin/sh

echo making two files ; sleep 1
touch file1 && touch file2
ls > file1 && cal >> file1 
sleep 1; echo redirect ls and cal into file1

echo /add to the end of the file >> file1

sleep 1 && echo capitalize the content of file1 and pipe it to file2
cat < file1 | tr a-z A-Z | tee file2
sleep 1; echo now copy file 2 to file 1 
cat file2  > file1 

sleep 1; cat file1 | tee file2 && (echo include pipes  && echo connectors directions && precedence)


rm file1 file2