//
// Created by garcia on 5/10/20.
//

#ifndef TOMASULO_INSTRUCTION_H
#define TOMASULO_INSTRUCTION_H


#include "util.h"

class Instruction {
public:
    int op = 0;
    int reg[3];
    int rsPos = -1;
    int fuPos = -1;

    int t_emit = -1;
    int t_ex = -1;
    int t_wb = -1;
};


#endif //TOMASULO_INSTRUCTION_H
