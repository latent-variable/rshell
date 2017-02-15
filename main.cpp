////////////////////////////////////////////////////////////////////
//Lino Gonzalez Valdovinos
//
//





#include <iostream>
#include <vector> 
#include <unistd.h>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include "Execute.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <boost/tokenizer.hpp>


using namespace std;



void print();
void pares(Command*&, string);
int main(){

    
    string input;
    
    do
    {
        
     
        print();
        getline(cin, input);
        Command* cmdvec = new Command();
        
        
        
        /*
        Mandate* cmd = new Mandate(input, "&&");
        Mandate* cmd2 = new Mandate("ls -l", "||");
        Mandate* cmd3 = new Mandate("echo done", ";");
        Mandate* cmd4 = new Mandate("echo this is the end", ";");
        And* N = new And(cmd,cmd2);
        Or* r = new Or(N,cmd3);
        Semicolon* S = new Semicolon(r, cmd4);
        S->Execute();
        
       */
       
       
       Mandate* man = new Mandate();
      
        if(input != "exit"){
            
            pares(cmdvec,input);
            
                man = cmdvec->getCommand(0);

        
            if (cmdvec->size() > 1)
                cmdvec->Execute();
                
            else
                man->Execute();
        }
        
        //exit will end shell
       
    
    }while(input != "exit");
   
    return 0;
}


void print()
{
     cout << "$ ";
}
void pares(Command*& cmdvec,string input){
    
    vector<string> c;
    int j = 0;
    
    for(unsigned int i = 0; i < input.size(); i++)
        {
            if(input.at(i) == '&' && input.at(i + 1) == '&')
            {
                c.push_back("&&");
                
            }
            else if(input.at(i) == '|' && input.at(i + 1) == '|')
            {
                c.push_back("||");
                
            }
            else if(input.at(i) == ';')
            {
                c.push_back(";");
                
            }
            
        }
        c.push_back("end");    
        
        char *str = const_cast<char *>(input.c_str());
        char* tok = strtok(str,"&|;\n");
        while(tok != NULL)
        {
            Mandate* cmd = new Mandate();    
                
            if(tok[0] != ' ')
            {   
                cmd->setConnector(c.at(j));
                cmd->setExecutable(tok);
                cmdvec->setCommand(cmd);
                j++;
                
            }
            else
            {
                
                cmd->setConnector(c.at(j));
                cmd->setExecutable(tok+1);
                cmdvec->setCommand(cmd);
                j++;
                
                
            }   
            
            tok = strtok(NULL, "&|;");
        }
        
    
}    