#ifndef EXECUTE_H
#define EXECUTE_H
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

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
class Mandate : public Base{
    private:
        string Executeble;
        string Connector;
       

    public:
        Mandate();
        Mandate(string,string);
        void Execute();
        void setChild(Mandate* );
        void setExecutable(string input);
        string getExecutable();
        void setConnector(string input);
        string getConnector();
};
////////////////////////////////
class Command : public Base{
    private:
        vector<Mandate*> commands;
        
    public:
        Command();
        Command(Mandate*);
        void setTree(Base*,unsigned int,Base*&);
        void Execute();
        void setCommand(Mandate* input);
        int size();
        Mandate* getCommand(int);
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