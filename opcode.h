#ifndef DTVM_OPCODE_H
#define DTVM_OPCODE_H

#include<stdlib.h>
#include "common.h"

dtvm_word dtvm_decode_arg(Opcode* const opcode, uint16_t nth)
{
    dtvm_word val = curr_proc->ram[curr_proc->ip + nth];
    switch(opcode->addr_modes[nth])
    {
    case AMODE_LIT: return val;
    case AMODE_REG: return curr_proc->regs[val];
    }
}
const size_t opcode_appetite = sizeof(Opcode) / sizeof(dtvm_word);

Operation dtvm_fetch_instr()
{
    Opcode opcode = *(Opcode*)(curr_proc->ram + curr_proc->ip);
    curr_proc->ip += opcode_appetite;
    Operation op;
    op.instr = opcode.instr;
    size_t i = 0;
    for(i = 0; i < MAX_ARGS; i++)
    {
        if(opcode.addr_modes[i] == AMODE_NOARG) break;
        if(curr_proc->sig_mode && op.instr != OP_CATCH)
            op.args[i] = 0;
        else
            op.args[i] = dtvm_decode_arg(&opcode, i);
    }
    curr_proc->ip += i;
    return op;
}

DTVM_Error dtvm_execute(Operation op)
{
    return opcodes[op.instr](op.args);
}

#endif