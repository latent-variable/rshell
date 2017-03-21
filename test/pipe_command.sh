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
cat file1 | tr a-z A-Z | tee file1
ls > file1 && cal >> file1 
echo \test the given \test \case \for this assigment && sleep 2

cat < file1 | tr A-Z a-z | tee file2 | tr a-z A-Z > file1

rm file1 file2

touch tesfile ; touch testfile2
sleep 1
echo testfile > testfile ; echo testfile2 > testfile2
sleep 1
ls -l > testfile ; (ls >> testfile && echo \n > testfile)
sleep 1
cat < tesfile && cat < testfile | tr a-z A-Z | tee testfile2
sleep 1
echo Rewrite > testfile2 && echo Rewrite > tesfile
sleep 1
(test what) && cat < what | tr a-z A-Z >> testfile
sleep 1
rm tesfile && rm testfile2