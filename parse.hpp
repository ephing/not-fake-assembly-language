#ifndef PARSE_HPP
#define PARSE_HPP

#include "instrs.hpp"
#include <fstream>
#include <stdexcept>
#include <unordered_map>

class Parse {
public:
    /**
     * Takes file and parses it
     * Maps labels to instructions and returns list of instructions
     */
    static std::vector<Instruction*>* parseFile(std::string filename, std::unordered_map<std::string,size_t>* labels) {
        auto instrs = new std::vector<Instruction*>();
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Your ASS file is bad! CRINGE!\n";
            return instrs;
        }
        std::string in;
        Instruction* instr = new Instruction();
        while (file >> in) {
            if (in.back() == ':') {
                if (labels->find(in.substr(0,in.size()-1)) == labels->end()) {
                    labels->insert(std::pair<std::string,size_t>(in.substr(0,in.size()-1),instrs->size()));
                } else throw std::runtime_error("Duplicate Label: " + in);
            }
            else if (in == "add") instr->setType(Instruction::Type::ADD);
            else if (in == "sub") instr->setType(Instruction::Type::SUB);
            else if (in == "mul") instr->setType(Instruction::Type::MUL);
            else if (in == "div") instr->setType(Instruction::Type::DIV);
            else if (in == "mod") instr->setType(Instruction::Type::MOD);
            else if (in == "neg") instr->setType(Instruction::Type::NEG);
            else if (in == "and") instr->setType(Instruction::Type::AND);
            else if (in == "or") instr->setType(Instruction::Type::OR);
            else if (in == "not") instr->setType(Instruction::Type::NOT);
            else if (in == "xor") instr->setType(Instruction::Type::XOR);
            else if (in == "call") instr->setType(Instruction::Type::CALL);
            else if (in == "j") instr->setType(Instruction::Type::JUMP);
            else if (in == "jif") instr->setType(Instruction::Type::JIF);
            else if (in == "ret") instr->setType(Instruction::Type::RETURN);
            else if (in == "gt") instr->setType(Instruction::Type::GT);
            else if (in == "gte") instr->setType(Instruction::Type::GTE);
            else if (in == "lt") instr->setType(Instruction::Type::LT);
            else if (in == "lte") instr->setType(Instruction::Type::LTE);
            else if (in == "eq") instr->setType(Instruction::Type::EQ);
            else if (in == "neq") instr->setType(Instruction::Type::NEQ);
            else if (in == "psh") instr->setType(Instruction::Type::PUSH);
            else if (in == "pop") instr->setType(Instruction::Type::POP);
            else {
                // if not a label or an instruction type, then we should be mid-instruction, meaning
                // that something is wrong if the type is still NONE
                // I was too lazy to tell you what was wrong though
                if (instr->getType() == Instruction::Type::NONE) {
                    throw std::runtime_error("Sum Ting Wong");
                    break;
                }
                // get all operands
                if (Instruction::opdCount[instr->getType()]) {
                    instr->addOpd(in);
                    for (int i = 0; i < Instruction::opdCount[instr->getType()] - 1; i++) {
                        file >> in;
                        instr->addOpd(in);
                    }
                }
                instrs->push_back(instr);
                instr = new Instruction();
            }
        }
        delete instr;
        file.close();
        return instrs;
    }
};

#endif