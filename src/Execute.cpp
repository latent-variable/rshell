#include "Execute.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>



/////////////////////////////////////////////////////////
//*******************Mandate****************************
////////////////////////////////////////////////////////

Mandate::Mandate(string a, string b)
{
    this->Executeble = a;
    this->Connector = b;
    this->Priority = 0;
}

void Mandate::Execute()
{
    //Check for test and add functionality 
    int status;
    int count = 1;
    string testflag = "empty";
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
    
    //checks if the command is test and if so then checks if the given path exist
    //if the command is not test then it executs the execvp
    if(command == "test" || command == "t")
    {
        struct stat s;
        char *args;
        if(argument[2] == "-e" || argument[2] == "-f" || argument[2] == "-d")
        {
            args = (char*)argument[3].c_str();
            testflag = argument[2];
        }
        else
        {
            args = (char*)argument[2].c_str();
        }
        
        if(testflag == "-e" || testflag == "empty")
        {
            if(stat(args, &s) == 0)
            {
                Flag = true;
                cout << "(TRUE)" << endl;
            }
            else
            {
                Flag = false;
                perror("Error!");
            }
        }
        else if(testflag == "-f")
        {
            if(stat(args, &s) == 0)
            {
                if(s.st_mode & S_IFREG)
                {
                    Flag = true;
                    cout << "(TRUE)" << endl;
                }
                else
                {
                    Flag = false;
                    cout << "(FALSE)" << endl;
                }
            }
            else
            {
                Flag = false;
                perror("Error!");
            }
        }
        else if(testflag == "-d")
        {
            if(stat(args, &s) == 0)
            {
                if(s.st_mode & S_IFDIR)
                {
                    Flag = true;
                    cout << "(TRUE)" << endl;
                }
                else
                {
                    Flag = false;
                    cout << "(FALSE)" << endl;
                }
            }
            else
            {
                Flag = false;
                perror("Error!");
            }
        }
    }
    else
    {
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
                while(count > 2)
                {
                    args[i]  = (char*) argument[i+1].c_str();
                    i++;
                    count--;
                    
                }
                args[i] = NULL;
                if( execvp(args[0], args) == -1)
                {
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

void Command::setTree(Base* item ,unsigned int i, Base*& out )
{
    if (i < commands.size()-1 )
    {    //**********************************************************************************************
        //We first need to go through the vector and find the higest priority.
        //Rearrange the vector base on Priority
        //Then we will make the tree based on the priority of the mandates
        //Example highest priority is 3 
        //make all the mandates of priority 3 first then all the ones of two 
        // all the ones of 1 and finally all the ones of zero
    
        string connect = connections[i];
        
    
        if(connect == "&&")
        {       
            unsigned int n =i+1;
            setTree(new And(item , commands[n]),n,out);
        }
        else if(connect == "||")
        {
            unsigned int n =i+1;
            setTree(new Or(item, commands[n]),n,out);
        }
        else if(connect == ";" || connect == "end"){
             unsigned int n =i+1;
            setTree(new Semicolon(item , commands[n]),n,out);
        }
        else if ( connect == "back" )
        {
            
            
            if (i +2 <= connections.size()){
                i++;
                connect = connections[i];
                 if(connect == "&&")
                {   
                    connect = connections[i+1];    
                    if(connect == "&&")
                    {       
                        And* c1 = new And(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new And(item, c1),n,out);
                    }
                    else if(connect == "||")
                    {
                        Or* c1 = new Or(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new And(item, c1),n,out);
                    }
                    else if(connect == ";"){
                        Semicolon* c1 = new Semicolon(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new And(item, c1),n,out);
                    }
                
                }
                else if(connect == "||")
                {
                    connect = connections[i+1];    
                    if(connect == "&&")
                    {       
                        And* c1 = new And(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new Or(item, c1),n,out);
                    }
                    else if(connect == "||")
                    {
                        Or* c1 = new Or(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new Or(item, c1),n,out);
                    }
                    else if(connect == ";"){
                        Semicolon* c1 = new Semicolon(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new Or(item, c1),n,out);
                    }
                    
                }
                else if(connect == ";"){
                    connect = connections[i+1];    
                    if(connect == "&&")
                    {       
                        And* c1 = new And(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new Semicolon(item, c1),n,out);
                    }
                    else if(connect == "||")
                    {
                        Or* c1 = new Or(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new Semicolon(item, c1),n,out);
                    }
                    else if(connect == ";"){
                        Semicolon* c1 = new Semicolon(commands[i],commands[i+1]);
                        unsigned int n =i+1;
                        setTree(new Semicolon(item, c1),n,out);
                    }
                }
            }
                
        }
    }
    else
    {
        out = item;
    }
}

void Command::Execute()
{   
    Base * out;
    unsigned int i = 0;
    setTree(commands[0] , i, out);
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

void Command::ArrangePriority()
{
    Mandate* temp = new Mandate();
    
    int highPriority = 0;
    unsigned arrsize = commands.size();
    cout<< "size: " << arrsize <<endl;
    for (unsigned i = 0; i < arrsize   ; i++ ){
        temp = commands.at(i);
        
        if (highPriority <temp->getPriority())
        {
            highPriority = temp->getPriority();
            
        }
    }
    cout<< "High Priority: " << highPriority <<endl;
    
   
   vector<Mandate*> tempcmds;
   
   unsigned j = 0;
   bool track = false;
   while (highPriority >= 0 ){
        track = true;
        if (j < arrsize){
            temp =commands.at(j);
            if (temp -> getPriority() == highPriority)
            {
                if(j < commands.size()-1){
                    if(connections.size() == 0)
                        connections.push_back(temp->getConnector());
                    else if (temp->getConnector()== "back" ){
                        if(connections.back() != "back" )
                            connections.push_back(temp->getConnector());
                    }else{
                        connections.push_back(temp->getConnector());
                    }
                            
                }
                else if(temp->getConnector() != "back") {
                    
                    connections.push_back(temp->getConnector());
                }
                
                tempcmds.push_back(temp);
                j++;
            }
            else{
                j++;
            }
        }else{
           
            highPriority--;
            j = 0;
        }
   }
   if(track == true)
   {
        for (unsigned i = 0; i< connections.size(); i++){
            cout<<tempcmds[i]->getExecutable()<<" "<< connections[i]<<endl;
        }
        commands.swap(tempcmds);
   }
    
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

Parenthesis::Parenthesis(Base* child, string c)
{
    Flag = true;
    this->child = child;
    this->connector = c;
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