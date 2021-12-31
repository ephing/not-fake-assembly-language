#ifndef EMU_HPP
#define EMU_HPP

#include <stack>
#include <list>
#include <vector>
#include <string>
#include <unordered_map>

class Instruction;

struct ActRecord {
    ActRecord() {} //initial record
    ActRecord(size_t r,std::string l,int a) : retAddr(r), lbl(l), larg(a) {}
    size_t retAddr;
    std::vector<int> record;
    std::string lbl;
    int larg;
};

class Program {
public:
    Program(std::string);
    ~Program();
    int execute();
private:
    std::unordered_map<std::string,size_t>* _labels;
    std::vector<Instruction*>* _instructions;
    std::stack<ActRecord> _stack;
    std::unordered_map<std::string,int> _mem;
    std::unordered_map<std::string,int> _regs;
    std::unordered_map<std::string,std::unordered_map<int,int>> _cache;

    void evaluate(Instruction*);
    int getVal(std::string) const;
    int resType(std::string) const;
    int locType(std::string) const;
    void set(std::string, int);
    void jump(std::string);
    size_t _current;
};

#endif