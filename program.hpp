#ifndef EMU_HPP
#define EMU_HPP

#include <stack>
#include <list>
#include <vector>
#include <string>
#include <unordered_map>

class Instruction;

/**
 * Stores return address, label, first argument, and the stack for function calls
 */
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
    // Initializes all the memories
    Program(std::string);
    ~Program();

    // Executes entire program
    int execute();
private:
    // maps labels to index of instructions in _instructions
    std::unordered_map<std::string,size_t>* _labels;

    // list of instructions output from Parse
    std::vector<Instruction*>* _instructions;

    // stack of activation records
    std::stack<ActRecord> _stack;

    // maps register names to the values stored in them
    std::unordered_map<std::string,int> _regs;

    // maps memory location to value stored there (implemented identically to _regs, so not particularly useful)
    std::unordered_map<std::string,int> _mem;

    // "cache" that stores outputs of functions with $ in the label
    std::unordered_map<std::string,std::unordered_map<int,int>> _cache;

    // Evaluates a single instruction
    void evaluate(Instruction*);

    // Handles retrieving memory from the various types of memory
    int getVal(std::string) const;

    /**
     * Determines what type of memory the string is referring to
     * @returns 1 when its a register, 2 when its mem, 3 when its stack, 0 otherwise
     */
    int locType(std::string) const;

    // Handles assigning to the various types of memory
    void set(std::string, int);

    // Takes label, sets _current to the appropriate location, or kills program if bad label
    void jump(std::string);
    size_t _current;
};

#endif