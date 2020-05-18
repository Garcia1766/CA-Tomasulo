//
// Created by garcia on 5/10/20.
//

#ifndef TOMASULO_RESSTATION_H
#define TOMASULO_RESSTATION_H

#include <cstdint>
#include "util.h"

class ResStation {
public:
    bool busy = false;
    int op = 0;
    int32_t vj = 0;
    int32_t vk = 0;
    int qj = -1;
    int qk = -1;
    int inst_cnt = -1;
    // int fuPos = -1;
    int emit_cycle = -1;
    int ready_cycle = -1;
};


#endif //TOMASULO_RESSTATION_H
