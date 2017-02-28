////////////////////////////////////////////////////////////////////
//name1=Lino Gonzalez Valdovinos
//ucrnetid1=lgonz041
//name2=Rafael Gomez
//ucrnetid2=rgome020
//https://github.com/lgonz041/rshell.git
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <algorithm> 
#include <cstring>
#include "Execute.h"
#include <list>

using namespace std;

void print();
void pares(Command*&, string);
int main(){
  
    string input;

    do{
        input = " ";
        print();
        getline(cin, input);
	if (input != ""){
           Command* cmdvec = new Command();
           Mandate* man = new Mandate();
       
           if(input != "exit"){
              pares(cmdvec,input);
              man = cmdvec->getCommand(0);
              
              if (cmdvec->size() > 1)
              {
                  cmdvec->ArrangePriority();
                  cmdvec->Execute();
              }   
              else
                  man->Execute();
	  }
	}
    }while(input != "exit");
   
    return 0;
}


void print()
{   
    //hopefully we can add user data
    char userName[64];
    int n = getlogin_r(userName, sizeof(userName)-1);
    /*if(0 != n)    //this way it will not print error for no username
    {
        perror("error");
    }*/
    n=n;
    cout << userName;
    cout << " $ ";
}

/////////////////////////////////////////
//Paresing function returns by refrence a 
//vector of type Mandate which hold the 
//commands and arguments in one string 
//and the connecters in another.
//////////////////////////////////////// 
//////////////////////////////////////// 
void pares(Command*& cmdvec,string input){
    
    //this takes out all the comments that are in the string
    //in turns "ls && ls #hhhhhhhhh# || ls" into "ls && ls  || ls"
    string input2 = "";
    bool flag = true;
    for(unsigned int i = 0; i < input.size(); i++){
        if(input.at(i) != '#' && flag == true){
            input2 += input.at(i);
        }
        else if(input.at(i) == '#' && flag == true){
            flag = false;
        }
        else if(input.at(i) == '#' && flag == false){
            flag = true;
        }
    }
    input = input2;
    //////////////////////////////////////////////////////////////
    //The string now removes the all the a "(" when it runs into it 
    //but also increase Priority. Also for ")" remove from string 
    //and increase priority
    
    vector<string> c;
    int priority = 0;
    int priority2 = 0;
    
    list<int> allpriority ;
    list<int>::iterator it = allpriority.begin();
    unsigned int size = input.size();
    for(unsigned int i = 0; i < size; i++)
    {
         if(input.at(i) == '(' )
        {
            priority++;
            priority2++;
            input.replace(i,1," ");
        }
        if(input.at(i) == ')' )
        {
            c.push_back("back");  
            priority2--;
            input.replace (i,1," ");
        }
        if(input.at(i) == '[')
        {
            input.replace(i,1,"t");
        }
        if(input.at(i) == ']')
        {
            input.replace(i,1," ");
        }
        if(input.at(i) == '&' && input.at(i + 1) == '&')
        {
            c.push_back("&&");
            allpriority.push_back( priority );
            priority = priority2;
        }
        else if(input.at(i) == '|' && input.at(i + 1) == '|')
        {
            c.push_back("||");
            allpriority.push_back( priority );
            priority = priority2;
        }
        else if(input.at(i) == ';')
        {
            c.push_back(";");
            allpriority.push_back( priority );
            priority = priority2;
        }
    }
    c.push_back("end");    
    allpriority.push_back( priority);
    int j = 0;
    
    char *str = const_cast<char *>(input.c_str());
    char* tok = strtok(str,"&|;\n");
    while(tok != NULL)
    {
        
        Mandate* cmd = new Mandate(); 
        
        if(tok[0] != ' ')
        {   
            advance(it,1);
            cmd->setConnector( c.at(j) );
            cmd->setExecutable( tok );
            cmd->setPriority(*it );  //passing mandate priority
            cout<<tok<< " priority: "<< *it <<" connector "<<c.at(j) <<endl;
            cmdvec->setCommand( cmd );
            
            j++;
        }
        else
        {   
            advance(it,1);
            cmd->setConnector(c.at(j));
            cmd->setExecutable(tok+1);
            cmd->setPriority( *it );  //passing mandate priority
            
            cout<<tok<<" priority: "<< *it <<" connector "<<c.at(j) <<endl;
            cmdvec->setCommand(cmd);
            j++;
        }   
        tok = strtok(NULL, "&|;");
    }

} 
