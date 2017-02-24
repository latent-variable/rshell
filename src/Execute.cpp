#include "Execute.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

/////////////////////////////////////////////////////////
//*******************Mandate****************************
////////////////////////////////////////////////////////

Mandate::Mandate(string a, string b)
{
    this->Executeble = a;
    this->Connector = b;
}

void Mandate::Execute()
{
    //Check for test and add functionality 
    
    
    int status;
    int count = 1;
    string command;
    string argument[50];
    
    char *str = const_cast<char *>(this->Executeble.c_str());
    char* token = strtok(str," ");
    while(token != NULL)
    {
        if(count == 1)
        {
            command = token;
            count++;
        }
        else
        {
            argument[count] = token;
            count++;
        }
        token = strtok(NULL, " ");
    }
    
    if(command == "exit"){
        exit(0);
    }
    
    pid_t pid; 
    //pid_t wpid;
    pid = fork();
    
    if ( pid < 0  ) 
    {   
        cout <<"Error running fork."<<endl;
       
    }
    if (pid == 0)//child
    {   
        
        if(count==2)
        {
            char *args[2];
            args[0]  = (char*) command.c_str();
            args[1] = NULL;
            if(execvp(args[0], args) == -1)
            {
                perror("Error!");
                exit(errno);
            }
        }
        else
        {
            //count -1 would be more ideal then 50
            char *args[50];
            args[0]  = (char*) command.c_str();
            int i = 1;
            while(count > 2){
                args[i]  = (char*) argument[i+1].c_str();
                i++;
                count--;
                
            }
            args[i] = NULL;
            if( execvp(args[0], args) == -1){
                perror("Error!");
                exit(errno);
            }
        }
    }
    else//parent
    {   
        do{
            wait(&status);
            //if(WIFEXITED(status))
            //cout<<"child exited with = "<<WEXITSTATUS(status)<<endl;        
            if (WEXITSTATUS(status) > 0)
                setBFlag(false) ;
            else
                setBFlag(true);
            // wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void Mandate::setExecutable(string input)
{
    this->Executeble = input;
}

string Mandate::getExecutable()
{
    return this->Executeble;
}

void Mandate::setConnector(string input)
{
    this->Connector = input;
}

string Mandate::getConnector()
{
    return this->Connector;
}
//Add methods for Priority
void Mandate::setPriority(int p)
{
    this->Priority = p;
}
int Mandate::getPriority()
{
    return this->Priority;
}
////////////////////////////////////////////////////////////////
//*************************Commands****************************
//////////////////////////////////////////////////////////////

Command::Command(){}

Command::Command(Mandate* item)
{
    commands.push_back(item);
}

void Command::setTree(Base* item ,unsigned int i, Base*& out, int currentpriority )
{
   if (i < commands.size()-1)
   {    //**********************************************************************************************
        //We first need to go through the vector and find the higest priority this 
        //should be done in the Command::Execute() method. Adding a int Priority parameter to setTree
        //Then we will make the tree based on the priority of the mandates
        //Example highest priority is 3 
        //make all the mandates of priority 3 first then all the ones of two 
        // all the ones of 1 and finally all the ones of zero
        //Whenever, the priority changes then we create a new Parenthesis object
        
        int highpriority = currentpriority;
        
        string connect = commands[i]->getConnector();
        int mandatePriority = commands[i]->getPriority();  //check for Priority change
        
        
        if(highpriority != mandatePriority )
        {
            unsigned int n = i + 1;
            setTree(new Parenthesis(item),n,out,mandatePriority);
        }
        if(connect == "&&")
        {       
            unsigned int n =i+1;
            setTree(new And(item , commands[n]),n,out,mandatePriority);
        }
        else if(connect == "||")
        {
            unsigned int n =i+1;
            setTree(new Or(item, commands[n]),n,out,mandatePriority);
        }
        else if(connect == ";")
        {
            unsigned int n = i+1;
            if(connect != "end")
                setTree(new Semicolon(item , commands[n]),n,out,mandatePriority);
            else
                setTree(new Semicolon(item , NULL),n,out,mandatePriority);
        }
    }
    else
    {
        out = item;
    }
}

void Command::Execute()
{   
    int higestpriority = 0;
    Base * out;
    unsigned int i = 0;
    for(unsigned int i = 0; i < commands.size()-1; i++){
        if (commands[i]->getPriority() > higestpriority)
            higestpriority = commands[i]->getPriority();
    }

    setTree(commands[0] , i, out, higestpriority);
    out->Execute();
}

void Command::setCommand(Mandate* input)
{
    commands.push_back(input);
}

int Command::size()
{
    return commands.size();
}

Mandate* Command::getCommand(int a)
{
    return (commands.at(a));
}

/////////////////////////////////////////////////////////////
//**********************AND Methods*************************
////////////////////////////////////////////////////////////

And::And(Base* child1, Base* child2)
{
    Flag = true;
    this->child1 = child1;
    this->child2 = child2;
}

void And::Execute()
{
    this->child1->Execute();
    if (child1->getBFlag() == true){
        this->child2->Execute();
        if(this->child2->getBFlag() == false)
            this->setBFlag(false);
    }
    else
        this->setBFlag(false);
}

/////////////////////////////////////////////////////
//*********************OR methods*******************
////////////////////////////////////////////////////

Or::Or(Base* child1, Base* child2)
{
    Flag = true;
    this->child1 = child1;
    this->child2 = child2;
}

void Or::Execute()
{
    this->child1->Execute();
    if ( child1->getBFlag() == false ){
        this->child2->Execute();
        if (child2->getBFlag() == false)
            this->setBFlag(true );
    }       
    else
        this->setBFlag( true );
}

////////////////////////////////////////////////////////
//****************Semicolon Methods********************
///////////////////////////////////////////////////////

Semicolon::Semicolon(Base* child1, Base* child2)
{ 
    Flag = true;
    this->child1 = child1;
    this->child2 = child2;
}

void Semicolon::Execute()
{
    child1->Execute();
    child2->Execute();
    if(child2->getBFlag() == false ){
        this->setBFlag( false );
    }
        
}

////////////////////////////////////////////////////////
//****************Parenthesis Methods******************
///////////////////////////////////////////////////////

Parenthesis::Parenthesis(Base* child)
{
    Flag = true;
    this->child = child;
}

void Parenthesis::Execute()
{
    child->Execute();
    if(child->getBFlag() == false)
    {
        this->setBFlag(false);
    }
    else
    {
        this->setBFlag(true);
    }
}