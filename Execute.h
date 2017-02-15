#ifndef EXECUTE_H
#define EXECUTE_H
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

class Base{
    public:
        Base(){};
        virtual void Execute() = 0;
};

class Mandate : public Base{
    private:
        bool mandateFlag;
        string Executeble;
        string Connector;
        Base* child;
        
        
    public:
        Mandate();
        Mandate(string,string);
        void Execute();
        void setChild(Mandate* );
        void setFlag(bool);
        bool getFlag();
        void setExecutable(string input);
        string getExecutable();
        void setConnector(string input);
        string getConnector();
};
////////////////////////////////
class Command : public Base{
    private:
        bool FlagExecute;
        bool done;
        vector<Mandate*> commands;
        
    public:
        Command();
        Command(Mandate*);
        void setTree(Base*,int,Base*&);
        void Execute();
        void setFlag(bool);
        bool getFlag();
        void setCommand(Mandate* input);
        bool getexit();
        void setexit(bool done);
        int size();
        Mandate* getCommand(int);
};
////////////////////////////////
class And: public Base{
    private:
        Base* child1;
        Base* child2;
        bool andFlag;
        
    public:
        And();
        And(Base*, Base*);
        void Execute();
        void setFlag(bool);
        bool getFlag();
};
///////////////////////////////
class Or: public Base{
    private:
        Base* child1;
        Base* child2;
        bool orFlag;
        
    public:
        Or(Base*, Base*);
        void Execute();
        void setFlag(bool);
        bool getFlag();
};
///////////////////////////////
class Semicolon: public Base{
    private:
        Base* child1;
        Base* child2;
        bool semiFlag;
        
    public:
        Semicolon(Base*, Base*);
        void Execute();
        void setFlag(bool);
        bool getFlag();
};
#endif 