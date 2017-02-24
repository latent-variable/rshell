#ifndef EXECUTE_H
#define EXECUTE_H
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

////////////////////////////////////////
//Base pure vitual class 
//holds the pure vitual method Execute 
//the Flag for all other classes to inherite
class Base{
    public:
        Base(){
            Flag = true;
        };
        virtual void Execute() = 0;
    	void setBFlag(bool f){
    	    Flag = f;
    	}
        bool getBFlag(){
            return Flag;
        }
    protected:
        bool Flag;
        
};
/////////////////////////////////////////////
//lowest level class(leaf of binary tree) 
//holds the  commands and arguments in 
//the string Executable and the connecters 
//in string Connectors.
//Each mandate knows how to execute its self
//build in to the execute fuction
class Mandate : public Base{
    private:
        string Executeble;
        string Connector;
        int Priority;
        //***************************************************
        //Addition of Priority to implement () paranthesis
        //When we look at the input everytime we see "(" the
        //Priority for that Mandate will +1
        //When we see ")" the Priority will decrease -1
        

    public:
        Mandate(){Priority = 0; };
        Mandate(string,string);
        void Execute();
        void setChild(Mandate* );
        void setExecutable(string input);
        string getExecutable();
        void setConnector(string input);
        string getConnector();
        void setPriority(int);
        int getPriority();
};
/////////////////////////////////////////////
//Commad holds a vector of Mandates(List 
//of all arguments and connetors) Then 
//builts ths bynary three in execute 
//which calls the setTree methods with 
//and builts it self from the bottom up
//recursively and returns by refrence pointer
//to the top ofthe tree.
class Command : public Base{
    private:
        vector<Mandate*> commands;
        
    public:
        Command();
        Command(Mandate*);
        void setTree(Base*,unsigned int,Base*&,int);
        void Execute();
        void setCommand(Mandate* input);
        Mandate* getCommand(int);
        int size();
        
};
////////////////////////////////
class And : public Base{
    private:
        Base* child1;
        Base* child2;
        
    public:
        And(Base*, Base*);
        void Execute();
};
///////////////////////////////
class Or: public Base{
    private:
        Base* child1;
        Base* child2;
        
    public:
        Or(Base*, Base*);
        void Execute();
};
///////////////////////////////
class Semicolon : public Base{
    private:
        Base* child1;
        Base* child2;
        
    public:
        Semicolon(Base*, Base*);
        void Execute();
};
////////////////////////////////////////////////
//Addition to the leaf pointer 
// [   ] or test
//returns 0 or true if file or folder exist 
/*
class Test :public Base{
    private:
        string Executeble;
        string Connector;
        
    public:
        Test(string, string);
        void Execute();
};*/ //ignore this, its easier and
//makes more sence to implement Test in the execute of mandate 
//since its a leaf with a executeble and a connector
//just need to check for "test " or "[" "]"
//and add functionality. Mainting the same structure.
///////////////////////////////
class Parenthesis : public Base{
    private:
        Base* child;
        string connector;
    public:
        Parenthesis(Base*);
        void Execute();
};

#endif 