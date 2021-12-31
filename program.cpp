#include "program.hpp"
#include "parse.hpp"

Program::Program(std::string filename) {
    _labels = new std::unordered_map<std::string,size_t>();
    _regs["@exitcode"] = 0;
    _stack.push(ActRecord());
    _instructions = Parse::parseFile(filename,_labels);
}

Program::~Program() {
    for (auto i : *_instructions) delete i;
    delete _instructions;
    delete _labels;
}

int Program::execute() {
    jump("_start");
    while (_current != _instructions->size()) evaluate(_instructions->at(_current));
    return _regs["@exitcode"];
}

void Program::evaluate(Instruction* instr) {
    bool jmp = false;
    switch (instr->getType()) {
    case Instruction::Type::ADD: {
        int val1,val2;
        /**
         * Instructions cant store into an immediate obviously, this makes sure the first operand is actually memory
         * I chose to do this in runtime instead of parsing because its faster.
         * If you dont want runtime errors, just write better assembly code
         */
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 + val2);
        }
        break;
    case Instruction::Type::SUB: {
        int val1, val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 - val2);
        }
        break;
    case Instruction::Type::MUL: {
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 * val2);
        }
        break;
    case Instruction::Type::DIV: {
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        if (!val2) std::runtime_error("Divide by 0");
        set(instr->getOpds()->front(), val1 / val2);
        }
        break;
    case Instruction::Type::MOD: {
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        if (!val2) std::runtime_error("Divide by 0");
        set(instr->getOpds()->front(), val1 % val2);
        }
        break;
    case Instruction::Type::NEG:{
        int val;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val = getVal(instr->getOpds()->at(1));
        set(instr->getOpds()->front(), -val);
        }
        break;
    case Instruction::Type::AND: {
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 & val2);
        }
        break;
    case Instruction::Type::OR: {
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 | val2);
        }
        break;
    case Instruction::Type::NOT:{
        int val;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val = getVal(instr->getOpds()->at(1));
        set(instr->getOpds()->front(), val == 0 ? 1 : 0);
        }
        break;
    case Instruction::Type::XOR: {
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 ^ val2);
        }
        break;
    case Instruction::Type::CALL:
        //check if this function has function has been "cached" with this input before
        if (_cache[instr->getOpds()->front()].find(_regs["@arg"]) != _cache[instr->getOpds()->front()].end()) {
            _regs["@ret"] = _cache[instr->getOpds()->front()][_regs["@arg"]];
        } else {
            // if the label ends with a $, that means this is a pure function, which means we can cache it
            if (instr->getOpds()->front().back() == '$') _cache[instr->getOpds()->front()][_regs["@arg"]] = -1;
            jmp = true;
            _stack.push(ActRecord(++_current,instr->getOpds()->front(),_regs["@arg"]));
            jump(instr->getOpds()->front());
        }
        break;
    case Instruction::Type::JUMP:
        jmp = true;
        jump(instr->getOpds()->front());
        break;
    case Instruction::Type::JIF:
        if (getVal(instr->getOpds()->front())) {
            jmp = true;
            jump(instr->getOpds()->at(1));
        }
        break;
    case Instruction::Type::RETURN:
        jmp = true;
        _regs["@ret"] = getVal(instr->getOpds()->front());
        // if $, cache result
        if (_stack.top().lbl.back() == '$') {
            _cache[_stack.top().lbl][_stack.top().larg] = _regs["@ret"];
        }
        _current = _stack.top().retAddr;
        _stack.pop();
        break;
    case Instruction::Type::GT:{
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 > val2 ? 1 : 0);
        }
        break;
    case Instruction::Type::GTE:{
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 >= val2 ? 1 : 0);
        }
        break;
    case Instruction::Type::LT:{
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 < val2 ? 1 : 0);
        }
        break;
    case Instruction::Type::LTE:{
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 <= val2 ? 1 : 0);
        }
        break;
    case Instruction::Type::EQ:{
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 == val2 ? 1 : 0);
        }
        break;
    case Instruction::Type::NEQ:{
        int val1,val2;
        int type = locType(instr->getOpds()->front());
        if (type == 0) throw std::runtime_error("Attempt to store operation result to non-register");
        val1 = getVal(instr->getOpds()->at(1));
        val2 = getVal(instr->getOpds()->at(2));
        set(instr->getOpds()->front(), val1 != val2 ? 1 : 0);
        }
        break;
    case Instruction::Type::PUSH: {
        int val = getVal(instr->getOpds()->front());
        _stack.top().record.push_back(val);
        break;
    }
    case Instruction::Type::POP:
        if (_stack.empty()) throw std::runtime_error("Attempt to pop from empty stack.");
        if (_stack.top().record.empty()) throw std::runtime_error("Attempt to pop from empty activation record.");
        _stack.top().record.pop_back();
        break;
    default: break;
    }
    if (!jmp) _current++;
}

int Program::getVal(std::string loc) const {
    if (locType(loc) == 1) {
        if (_regs.find(loc) != _regs.end()) return _regs.at(loc);
        else throw std::runtime_error("Bad register");
    }
    if (locType(loc) == 2) {
        if (_mem.find(loc) != _mem.end()) return _mem.at(loc);
        else throw std::runtime_error("Bad memory location");
    }
    if (locType(loc) == 3) {
        size_t l = std::stoi(loc.substr(1,loc.size()-1));
        if (_stack.top().record.size() - 1 < l) throw std::runtime_error("Invalid stack access");
        return _stack.top().record.at(l);
    }
    else return std::stoi(loc);
}

void Program::jump(std::string label) {
    if (_labels->find(label) != _labels->end()) _current = _labels->at(label);
    else throw std::runtime_error("Jump to invalid label: " + label);
}

int Program::locType(std::string loc) const {
    if (loc.front() == '@') return 1;
    if (loc.front() == '#') return 2;
    if (loc.front() == '!') return 3;
    return 0;
}

void Program::set(std::string loc, int val) {
    if (locType(loc) == 1) _regs[loc] = val;
    if (locType(loc) == 2) _mem[loc] = val;
    if (locType(loc) == 3) {
        size_t l = std::stoi(loc.substr(1,loc.size()-1));
        _stack.top().record[l] = val;
    }
}