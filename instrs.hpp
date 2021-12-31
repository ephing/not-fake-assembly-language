#ifndef INSTRS_HPP
#define INSTRS_HPP

#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

class Instruction {
public:
    enum class Type {
        ADD,SUB,MUL,DIV,MOD,NEG,
        AND,OR,NOT,XOR,
        CALL,JUMP,JIF,RETURN,
        GT,GTE,LT,LTE,EQ,NEQ,
        PUSH,POP,
        NONE
    };
    Instruction(): _type(Type::NONE), _opds(new std::vector<std::string>()) {}
    ~Instruction() {
        delete _opds;
    }
    void setType(Type t) { _type = t; }
    Type getType() const { return _type; }
    void addOpd(std::string opd) { _opds->push_back(opd); }
    std::vector<std::string>* getOpds() const { return _opds; }
    void print() const {
        std::cout << static_cast<int>(_type) << " ";
        for (auto i : *_opds) std::cout << i << " ";
        std::cout << "\n";
    }
    static std::unordered_map<Type,int> opdCount;
private:
    Type _type;
    std::vector<std::string>* _opds;
};

/**
 * This is a mapping from instruction type to number of operands it should have
 * If you don't follow this map, the parser will cry
 */
std::unordered_map<Instruction::Type,int> Instruction::opdCount = std::unordered_map<Type,int>({
    std::pair<Type,int>(Type::ADD,3),
    std::pair<Type,int>(Type::SUB,3),
    std::pair<Type,int>(Type::MUL,3),
    std::pair<Type,int>(Type::DIV,3),
    std::pair<Type,int>(Type::MOD,3),
    std::pair<Type,int>(Type::NEG,2),
    std::pair<Type,int>(Type::AND,3),
    std::pair<Type,int>(Type::OR,3),
    std::pair<Type,int>(Type::NOT,2),
    std::pair<Type,int>(Type::XOR,3),
    std::pair<Type,int>(Type::CALL,1),
    std::pair<Type,int>(Type::JUMP,1),
    std::pair<Type,int>(Type::JIF,2),
    std::pair<Type,int>(Type::RETURN,1),
    std::pair<Type,int>(Type::GT,3),
    std::pair<Type,int>(Type::GTE,3),
    std::pair<Type,int>(Type::LT,3),
    std::pair<Type,int>(Type::LTE,3),
    std::pair<Type,int>(Type::EQ,3),
    std::pair<Type,int>(Type::NEQ,3),
    std::pair<Type,int>(Type::PUSH,1),
    std::pair<Type,int>(Type::POP,0),
    std::pair<Type,int>(Type::NONE,0),
});

#endif