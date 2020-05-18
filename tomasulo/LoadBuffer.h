//
// Created by garcia on 5/10/20.
//

#ifndef TOMASULO_LOADBUFFER_H
#define TOMASULO_LOADBUFFER_H

#include <cstdint>

class LoadBuffer {
public:
    bool busy = false;
    int32_t address = 0;
    int inst_cnt = -1;
    // int fuPos = -1;
    int emit_cycle = -1;
    int ready_cycle = -1;
};


#endif //TOMASULO_LOADBUFFER_H
