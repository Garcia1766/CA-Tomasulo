//
// Created by garcia on 5/10/20.
//

#ifndef TOMASULO_TOMASULO_H
#define TOMASULO_TOMASULO_H


#include "ResStation.h"
#include "LoadBuffer.h"
#include "FuncUnit.h"
#include "RegStat.h"
#include "Instruction.h"

class Tomasulo {
public:
    ResStation resStation[9];  // 0 <= Ars < 6, 6 <= Mrs < 9
    LoadBuffer loadBuffer[3];  // 0 <= LB < 3
    FuncUnit funcUnit[7];  // 0 <= Add < 3, 3 <= Mult < 5, 5 <= Load < 7
    RegStat regStat[32];
    Instruction *instruction;
    int num_inst;

    Tomasulo(Instruction *instruction1, int num_inst1);

    int clear_rs(int rsPos);
    bool rs_to_fu(int rsPos, int cycle);
    int rss_to_fu(int cycle);
    bool there_is_jump();

    void decrease_fu(int *pc, bool *flag, int cycle);
    bool emit_inst(int *pc, bool *flag, int cycle);

};


#endif //TOMASULO_TOMASULO_H
