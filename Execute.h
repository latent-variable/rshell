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
        bool Flag = true;
};

class Mandate : public Base{
    private:
        bool Flag = true;
        string Executeble;
        string Connector;
        
        
        
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
        bool Flag = true;
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
        bool Flag = true;
        
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
        bool Flag = true;
        
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
        bool Flag = true;
        
    public:
        Semicolon(Base*, Base*);
        void Execute();
        void setFlag(bool);
        bool getFlag();
};
#endif 