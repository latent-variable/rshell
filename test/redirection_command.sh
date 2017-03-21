#!/bin/sh
touch file1 && echo create a file && sleep 1

echo /add this to the file > file1 && echo added text to the file
sleep 1
echo /this will rewrite the file > file1
sleep 1 && echo replace the text \in the file
echo /now append this to the file >> file1
sleep 1; echo show content of the file
cat file1 

sleep 3;
cat < file1 && echo works as just like the previous commmand using \<
sleep 1 && cout send ls and cal to file and \then cat
ls >> file1

cal >> file1

cat < file1
sleep 1; echo now all at one 
sleep 1; cal > out && ls -a >> out && ls >> out && cat < file1  
sleep 3 && echo finally remove file 
rm file1

sleep 1
touch testfile ; echo created new flie
sleep 1
echo echoing this into file > testfile && echo and should have this >> tesfile
sleep 1
cat < testfile && echo rewrote into file > testfile
sleep 1
echo added more >> testfile && cat < testfile
sleep 1
rm testfile