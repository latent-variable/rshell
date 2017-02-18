////////////////////////////////////////////////////////////////////
//name1=Lino Gonzalez Valdovinos
//ucrnetid1=lgonz041
//name2=Rafael Gomez
//ucrnetid2=rgome020
//repourl=https://github.com/lgonz041/rshell.git
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
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
    }while(input != "exit");
   
    return 0;
}


void print()
{   
    //hopefully we can add user data
     cout << "$ ";
}

/////////////////////////////////////////
//Paresing function returns by refrence a 
//vector of type Mandate which hold the 
//commands and arguments in one string 
//and the connecters in another.
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

/*
test cases:
echo A #echo B echo A #&& echo B echo A && echo B #&& echo C echo A #; echo B echo A #|| echo B
exit #just exit echo A && exit #A echo A || exit #A, testdoneecho A; exit #A echo A #exit #A, testdone
echo A && echo B echo A || echo B echo A; echo B ls -j && echo A ls -j || echo A ls -j; echo A ls -j && echo A || echo B echo A || echo B || echo C echo A || echo B && echo C echo A && echo B && echo C  # print A, B, C
ls ls -al ls -j echo A echo AB CD help help pwd echo echo echa AB cal

*/