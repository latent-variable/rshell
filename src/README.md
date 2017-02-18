# rshell
cs-100 assignment 
Built the rshell using the Composite design pattern.
Base class is pure virtual with the fuction execute being set as pure vitual. 
Base class holds Flag for the other classes to inherite. 
The leaf class is Mandate which holds two strings one for the command and argument and one for the connecter. 
Then the Commands Class is a Composite of Mandates.
And, Or, and Semicolon are classes that are use are conecters in the tree.  

Known Bugs 
mkdir folder || echo done will make the folder the first time and not echo.
However, the second time it will not be able to make the folder but it will not
print done either. 

