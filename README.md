# not-fake-assembly-language
It's very real

## Usage:  
`./emulator <assembly file>`

## NFAL Spec:  
### Instructions:
#### Operands
There are 4 types of operands
* immediates  
    Integers written in plain text baby
* registers  
    This language has arbitrarily many registers with arbitrary names. Registers must be assigned
    to before they can be accessed. Once a register is created it lasts forever.
    Registers are denotated with an @ followed by the name: `@exitcode`
    There are 3 special registers:  
    * @exitcode  
        This is where the exitcode for the program is stored
    * @ret  
        This is for return values in functions
    * @arg  
        While any register can be used to pass arguments, pure functions with 1 argument must use @arg
* mem  
    Exactly the same as registers, literally not useful in the slightest.
    Mem is denotated with a tic tac toe board followed by the name: ex `#0`
* the stack (sorta)  
    Because I am an amazing programmer, the stack is actually not that fast in this emulator  
    The benefit of the stack is that it emulates scoping, while regs and mem are persistent  
    Stack variables are denotated with a bang followed by the offset into the current activation record: ex `!3`
#### Tertiary instructions
`inst o1 o2 o3`  
* add: o1 = o2 + o3
* sub: o1 = o2 - o3
* mul: o1 = o2 * o3
* div: o1 = o2 / o3
* mod: o1 = o2 % o3
* and: o1 = o2 & o3
* or: o1 = o2 | o3
* xor: o1 = o2 ^ o3
* gt: o1 = o2 > o3
* gte: o1 = o2 >= o3
* lt: o1 = o2 < o3
* lte: o1 = o2 <= o3
* eq: o1 = o2 == o3
* neq: o1 = o2 != o3  
o1 cannot be an immediate
#### Binary instructions
`inst o1 o2`  
* neg: o1 = -o2
* not: o1 = ~o2
* jif: jumps to o2 if o1 is not 0
#### Unary instructions
`inst o1`  
* call: create new activation record and jump to o1 (or, if o1 is pure, attempt to get cached result)
* j: jump to o1
* ret: Sets @ret, destroys activation record, caches pure function result, jumps to return address
* push: pushes o1 onto stack in current activation record
#### other
* pop: pops most recently pushed thing off the current activation record