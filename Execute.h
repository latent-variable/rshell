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

    public:
        Mandate(){ };
        Mandate(string,string);
        void Execute();
        void setChild(Mandate* );
        void setExecutable(string input);
        string getExecutable();
        void setConnector(string input);
        string getConnector();
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
        void setTree(Base*,unsigned int,Base*&);
        void Execute();
        void setCommand(Mandate* input);
        Mandate* getCommand(int);
        int size();
        
};
////////////////////////////////
class And: public Base{
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
class Semicolon: public Base{
    private:
        Base* child1;
        Base* child2;
       
        
    public:
        Semicolon(Base*, Base*);
        void Execute();
};     
#endif 