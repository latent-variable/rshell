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
#include "Execute.h"

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
                  cmdvec->Execute();
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
    
    vector<string> c;
    int priority= 0;
    int j = 0;
    
    for(unsigned int i = 0; i < input.size(); i++)
    {
         if(input.at(i) == '(' )
        {
            priority++;  
        }
        if(input.at(i) == ')' )
        {
            --priority;
        }
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
    char* tok = strtok(str,"&|;()\n");
    while(tok != NULL)
    {
        Mandate* cmd = new Mandate();    
        if(tok[0] != ' ')
        {   
            cmd->setConnector(c.at(j));
            cmd->setExecutable(tok);
            cmd->setPriority(priority);  //passing mandate priority
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
        tok = strtok(NULL, "&|;()");
    }
} 
/* 
    for(unsigned int i = 0; i < input.size(); i++)
    {
        if(input.at(i) == '(' && pflag == false)
        {
            pflag = true;   
        }
        else if(input.at(i) == ')' && pflag == true)
        {
            pflag = false;
        }
        if(pflag == false)
        {
            if(input.at(i) == '&' && input.at(i + 1) == '&')
            {
                outside.push_back("&&");
            }
            else if(input.at(i) == '|' && input.at(i + 1) == '|')
            {
                outside.push_back("||");
            }
            else if(input.at(i) == ';')
            {
                outside.push_back(";");
            }
        }
        else if(pflag == true)
        {
            if(input.at(i) == '&' && input.at(i + 1) == '&')
            {
                c2.push_back("&&");
            }
            else if(input.at(i) == '|' && input.at(i + 1) == '|')
            {
                c2.push_back("||");
            }
            else if(input.at(i) == ';')
            {
                c2.push_back(";");
            }
        }
    }
    outside.push_back("end");
    c2.push_back("end");
    
    for(unsigned int i = 0; i < outside.size(); i++){
        cout << "out: " << outside[i] << endl;
    }
    cout << endl << "c2:";
    for(unsigned int i = 0; i < c2.size(); i++){
        cout << "in: " << c2[i] << endl;
    }
    */