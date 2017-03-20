#include "Execute.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>



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
    //int status2;
    int count = 1;
    int count2 = 0;
    bool childflag = false ;
    string exec1 = this->Executeble;
    string exec2 = this->Executeble;
    string testflag = "empty";
    string command;
    string argument[100];
    //char* argument2[20];
    vector<string> commands;
    
    /////////////////////////////////////////////////////
    //add dup() and Pipe functionallity 
    ////////////////////////////////////////////////////
    //int fd[2];   //file descripter {0 stdin, 1 stdout, 2 stderr}
    
    char* str2 = const_cast<char *>(exec1.c_str());
    char* token2 = strtok(str2,"*");
    while(token2 != NULL)
    {
        //argument2[count2] = token2;
        commands.push_back( token2 );
        count2++;
        if(count2 == 2)
            childflag = true;
        token2 = strtok(NULL, "*");
    }

    char *str = const_cast<char *>(exec2.c_str());
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
        //pipe(fd);
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
                //redirect output to file instead of stdout
                for(int i= 0; i < count; i++){
                    //check all arguments for > and >> as well as for < 
                    if(argument[i] == ">" )
                    {
                        int savestdout;
                        int fds;
                        if((savestdout = dup(1) < 0))// creates duplicate of stdout and stores it in savestdout
                        {
                            perror("Error!");
                        }
                        
                        close(1);
                        
                        if((fds = open(argument[i+1].c_str(),O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR) < 0))// opens file and the file descriptor is given to fds
                        {
                            perror("Error!");
                        }
                        
                        if(dup2(fds,savestdout) < 0)// replaces stdout with new file fds
                        {
                            perror("Error!");
                        }
                        
                        count -= 2;
                    }
                    else if(argument[i] == ">>"  )
                    {   
                        int savestdout;
                        int fds;
                        
                        if((savestdout = dup(1) < 0))// creates duplicate of stdout and stores it in savestdout
                        {
                            perror("Error!");
                        }
                        
                        close(1);
                        
                        if((fds = open(argument[i+1].c_str(),O_WRONLY | O_APPEND) < 0))// opens file and the file descriptor is given to fds
                        {
                            perror("Error!");
                        }
                        
                        if(dup2(fds,savestdout) < 0)// replaces stdout with new file fds
                        {
                            perror("Error!");
                        }
                        
                        count -= 2;
                    }
                    else if( argument[i] == "<" )
                    {
                        int savestdin;
                        int fds;
                        
                        if((savestdin = dup(0) < 0))// creates duplicate of stdin and stores it in savestdout
                        {
                            perror("Error!");
                        }
                        
                        close(0);
                        
                        if((fds = open(argument[i+1].c_str(),O_RDONLY ) < 0))// opens file and the file descriptor is given to fds
                        {
                            perror("Error!");
                        }
                        
                        if(dup2(fds,savestdin) < 0) // replaces stdin with new file fds
                        {
                            perror("Error!");
                        }
                        
                        count -= 2;
                    }
                        
                }
                
                if(childflag == false ){
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
                if(childflag == true ){
                    
                   
                    pid_t pid;
                   
                    //array of pipes assuming we wont need more than 10
                    int fds[10][2];
                    //keeps track of pipe and commands
                    unsigned z=0;
  
                    for(unsigned i = 0; i < commands.size(); i++)
                    {
                        
                        if(pipe(fds[z]) < 0){
                            perror("pipe Error!");
                        }
                        
                        
                        //cout<< "-----------Pipe "<<z <<"---------- "<<endl;
                        
                        pid = fork();
                        
                        
                        if(pid < 0)
                        {
                            perror("fork Error!");
                        }
                        else if(pid == 0)//child
                        {
                            if(z != 0)//if its not the first command then get from previous command
                            {
                                close(fds[z-1][1]);
                                //cout<<"read from ["<<z-1<<"][" << 0<<"]"<<endl;
                                if(dup2(fds[z-1][0], 0) < 0)//since not its not the first command then we read
                                {
                                    perror("first dup Error!");
                                }
                            }
                    
                            if(z != commands.size()-1)//if its no the last command
                            {
                                close(fds[z][0]);
                                //cout<<"write too ["<<z<<"][" << 1<<"]"<<endl;
                                if(dup2(fds[z][1], 1) < 0)//since not last command we write to next command
                                {
                                    perror("second dup Error!");
                                }
                                
                            }
                            unsigned count3 = 0;
                            char * arg3[20];
                            char *str3 = const_cast<char *>(commands[z].c_str());
                            char* token3 = strtok(str3," ><");
                            while(token3 != NULL)
                            {
                                arg3[count3] = token3;
                                //cout<<count3 << " " << arg3[count3] <<endl;
                                count3++;
                              
                                token3 = strtok(NULL, " ><");
                            }
                            arg3[count3]=NULL;
                            
                            if(execvp(arg3[0], arg3) < 0)//run the commands
                            {
                                perror("exe Error!");
                            }
                        }else{//parent 
                              
                                 unsigned int microseconds=30000;
                                 usleep(microseconds);
                                 //crucial that we wait for child to finish and then close 
                                 //the used pipes 
                                 close(fds[z][1]);
                                
                        }
                        //must increase z 
                        z++;  
                    }
                        
                }
                    
                
            }
        }
        else//parent
        {
           do{
                wait(&status); 
                if (WEXITSTATUS(status) > 0)
                    setBFlag(false) ;
                else
                    setBFlag(true);
               
               
               unsigned int microseconds=20000;
                usleep(microseconds);
               
            
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
            if( i + 2 == connections.size() ){
                i++;
                connect = connections[i];
                if(connect == "&&")
                {       
                    unsigned int n =i+1;
                    setTree(new And(item , commands[i]),n,out);
                }
                else if(connect == "||")
                {
                    unsigned int n =i+1;
                    setTree(new Or(item, commands[i]),n,out);
                }
                else if(connect == ";" || connect == "end"){
                     unsigned int n =i+1;
                    setTree(new Semicolon(item , commands[i]),n,out);
                }
                
            }
            
            if (i + 2 < connections.size() ){
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
    setTree(commands[0], i, out);
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
    //cout<< "size: " << arrsize <<endl;
    for (unsigned i = 0; i < arrsize   ; i++ ){
        temp = commands.at(i);
        
        if (highPriority < temp->getPriority())
        {
            highPriority = temp->getPriority();
            
        }
    }
    //cout<< "High Priority: " << highPriority <<endl;
    
   
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
       /*
        for (unsigned i = 0; i< connections.size(); i++){
            cout<<tempcmds[i]->getExecutable()<<" "<< connections[i]<<endl;
        }*/
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