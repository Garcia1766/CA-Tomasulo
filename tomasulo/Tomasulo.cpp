//
// Created by garcia on 5/10/20.
//

#include "Tomasulo.h"
#include <vector>

Tomasulo::Tomasulo(Instruction *instruction1, int num_inst1) {
    instruction = instruction1;
    num_inst = num_inst1;
}

int Tomasulo::clear_rs(int rsPos) {
    int ret = 0;
    if (0 <= rsPos && rsPos < 9) {
        switch (resStation[rsPos].op) {
            case ADD:
                ret = resStation[rsPos].vj + resStation[rsPos].vk;
                break;
            case SUB:
                ret = resStation[rsPos].vj - resStation[rsPos].vk;
                break;
            case MULT:
                ret = resStation[rsPos].vj * resStation[rsPos].vk;
                break;
            case DIV:
                ret = resStation[rsPos].vk == 0 ? resStation[rsPos].vj : resStation[rsPos].vj / resStation[rsPos].vk;
                break;
        }
        resStation[rsPos].busy = false;
        resStation[rsPos].op = 0;
        resStation[rsPos].vj = 0;
        resStation[rsPos].vk = 0;
        resStation[rsPos].qj = -1;
        resStation[rsPos].qk = -1;
        instruction[resStation[rsPos].inst_cnt].rsPos = -1;
        resStation[rsPos].inst_cnt = -1;
    } else if (rsPos < 12) {
        ret = loadBuffer[rsPos - 9].address;
        loadBuffer[rsPos - 9].busy = false;
        loadBuffer[rsPos - 9].address = 0;
        instruction[loadBuffer[rsPos - 9].inst_cnt].rsPos = -1;
        loadBuffer[rsPos - 9].inst_cnt = -1;
    }
    return ret;
}

bool Tomasulo::rs_to_fu(int rsPos, int cycle) {  // check if rs is ready and fu is free, send one in rs to fu
    if (rsPos < 6) {  // ADD, SUB, JUMP
        if (resStation[rsPos].busy && resStation[rsPos].qj == -1 && resStation[rsPos].qk == -1 && instruction[resStation[rsPos].inst_cnt].fuPos == -1) {
            for (int i = 0; i < 3; ++i) {
                if (funcUnit[i].cyclesLeft < 0) {
                    funcUnit[i].op = resStation[rsPos].op;
                    funcUnit[i].cyclesLeft = resStation[rsPos].op == JUMP ? 1 : 3;
                    funcUnit[i].inst_cnt = resStation[rsPos].inst_cnt;
                    instruction[resStation[rsPos].inst_cnt].fuPos = i;
                    resStation[rsPos].ready_cycle = -1;
                    return true;
                }
            }
        }
    } else if (rsPos < 9) {  // MULT DIV
        if (resStation[rsPos].busy && resStation[rsPos].qj == -1 && resStation[rsPos].qk == -1 && instruction[resStation[rsPos].inst_cnt].fuPos == -1)
        for (int i = 3; i < 5; ++i) {
            if (funcUnit[i].cyclesLeft < 0) {
                funcUnit[i].op = resStation[rsPos].op;
                if (resStation[rsPos].op == DIV && resStation[rsPos].vk == 0)
                    funcUnit[i].cyclesLeft = 1;
                else
                    funcUnit[i].cyclesLeft = 4;
                funcUnit[i].inst_cnt = resStation[rsPos].inst_cnt;
                instruction[resStation[rsPos].inst_cnt].fuPos = i;
                resStation[rsPos].ready_cycle = -1;
                return true;
            }
        }
    } else {
        if (loadBuffer[rsPos - 9].busy && instruction[loadBuffer[rsPos - 9].inst_cnt].fuPos == -1) {  // LD
            for (int i = 5; i < 7; ++i) {
                if (funcUnit[i].cyclesLeft < 0) {
                    funcUnit[i].op = LD;
                    funcUnit[i].cyclesLeft = 3;
                    funcUnit[i].inst_cnt = loadBuffer[rsPos - 9].inst_cnt;
                    instruction[loadBuffer[rsPos - 9].inst_cnt].fuPos = i;
                    loadBuffer[rsPos - 9].ready_cycle = -1;
                    return true;
                }
            }
        }
    }
    return false;
}

int Tomasulo::rss_to_fu(int cycle) {
    int cnt = 0;
    while (1) {
        int smallest_ready_cycle = INT32_MAX;
        int target_emit = -1;
        for (int i = 0; i < 6; ++i) {
            if (resStation[i].ready_cycle > 0 && resStation[i].ready_cycle < smallest_ready_cycle)
                smallest_ready_cycle = resStation[i].ready_cycle;
        }
        vector<int> res_index;
        for (int i = 0; i < 6; ++i) {
            if (resStation[i].ready_cycle == smallest_ready_cycle)
                res_index.push_back(i);
        }
        if (res_index.empty())  // no ready
            break;
        int smallest_emit_cycle = INT32_MAX;
        for (int j = 0; j < res_index.size(); ++j) {
            if (resStation[res_index.at(j)].emit_cycle > 0 && resStation[res_index.at(j)].emit_cycle < smallest_emit_cycle) {
                smallest_emit_cycle = resStation[res_index.at(j)].emit_cycle;
                target_emit = res_index.at(j);
            }
        }
        if (target_emit < 0)
            break;
        if (!rs_to_fu(target_emit, cycle))
            break;
        cnt++;
    }

    while (1) {
        int smallest_ready_cycle = INT32_MAX;
        int target_emit = -1;
        for (int i = 6; i < 9; ++i) {
            if (resStation[i].ready_cycle > 0 && resStation[i].ready_cycle < smallest_ready_cycle)
                smallest_ready_cycle = resStation[i].ready_cycle;
        }
        vector<int> res_index;
        for (int i = 6; i < 9; ++i) {
            if (resStation[i].ready_cycle == smallest_ready_cycle)
                res_index.push_back(i);
        }
        if (res_index.empty())  // no ready
            break;
        int smallest_emit_cycle = INT32_MAX;
        for (int j = 0; j < res_index.size(); ++j) {
            if (resStation[res_index.at(j)].emit_cycle > 0 && resStation[res_index.at(j)].emit_cycle < smallest_emit_cycle) {
                smallest_emit_cycle = resStation[res_index.at(j)].emit_cycle;
                target_emit = res_index.at(j);
            }
        }
        if (target_emit < 0)
            break;
        if (!rs_to_fu(target_emit, cycle))
            break;
        cnt++;
    }

    while (1) {
        int smallest_ready_cycle = INT32_MAX;
        int target_emit = -1;
        for (int i = 0; i < 3; ++i) {
            if (loadBuffer[i].ready_cycle > 0 && loadBuffer[i].ready_cycle < smallest_ready_cycle)
                smallest_ready_cycle = loadBuffer[i].ready_cycle;
        }
        vector<int> lb_index;
        for (int i = 0; i < 3; ++i) {
            if (loadBuffer[i].ready_cycle == smallest_ready_cycle)
                lb_index.push_back(i);
        }
        if (lb_index.empty())
            break;
        int smallest_emit_cycle = INT32_MAX;
        for (int j = 0; j < lb_index.size(); ++j) {
            if (loadBuffer[lb_index.at(j)].emit_cycle > 0 && loadBuffer[lb_index.at(j)].emit_cycle < smallest_emit_cycle) {
                smallest_emit_cycle = loadBuffer[lb_index.at(j)].emit_cycle;
                target_emit = lb_index.at(j) + 9;
            }
        }
        if (target_emit < 0)
            break;
        if (!rs_to_fu(target_emit, cycle))
            break;
        cnt++;
    }
    return cnt;
}

void Tomasulo::decrease_fu(int *pc, bool *flag, int cycle) {
    for (int i = 0; i < 7; ++i) {
        if (funcUnit[i].cyclesLeft > 0) {  // decrease fu
            funcUnit[i].cyclesLeft--;
            if (funcUnit[i].cyclesLeft == 0 && instruction[funcUnit[i].inst_cnt].t_ex == -1)
                instruction[funcUnit[i].inst_cnt].t_ex = cycle;
        } else if (funcUnit[i].cyclesLeft == 0) {  // decrease to -1
            funcUnit[i].cyclesLeft--;
            int inst_rsPos = instruction[funcUnit[i].inst_cnt].rsPos;
            if (funcUnit[i].op == JUMP) {
                if (instruction[funcUnit[i].inst_cnt].reg[0] == resStation[inst_rsPos].vj)
                    *pc += instruction[funcUnit[i].inst_cnt].reg[2];
                else
                    *pc += 1;
                *flag = false;
                instruction[funcUnit[i].inst_cnt].fuPos = -1;  // clear fu
                clear_rs(inst_rsPos);  // clear rs
                if (instruction[funcUnit[i].inst_cnt].t_wb == -1)
                    instruction[funcUnit[i].inst_cnt].t_wb = cycle;
                continue;
            }
            instruction[funcUnit[i].inst_cnt].fuPos = -1;  // clear fu
            int ret = clear_rs(inst_rsPos);  // clear rs
            if (regStat[instruction[funcUnit[i].inst_cnt].reg[0]].source == inst_rsPos) {
                regStat[instruction[funcUnit[i].inst_cnt].reg[0]].source = -1;  // clear regStat
                regStat[instruction[funcUnit[i].inst_cnt].reg[0]].value = ret;
            }
            if (instruction[funcUnit[i].inst_cnt].t_wb == -1)
                instruction[funcUnit[i].inst_cnt].t_wb = cycle;

            for (int j = 0; j < 9; ++j) {
                if (inst_rsPos == resStation[j].qj) {
                    resStation[j].qj = -1;
                    resStation[j].vj = ret;
                    if (resStation[j].qk == -1)
                        resStation[j].ready_cycle = cycle;
                }
                if (inst_rsPos == resStation[j].qk) {
                    resStation[j].qk = -1;
                    resStation[j].vk = ret;
                    if (resStation[j].qj == -1)
                        resStation[j].ready_cycle = cycle;
                }
            }
        }
    }
    rss_to_fu(cycle);
}

bool Tomasulo::there_is_jump() {
    for (int i = 0; i < 6; ++i) {
        if (resStation[i].op == JUMP)
            return true;
    }
    return false;
}

bool Tomasulo::emit_inst(int *pc, bool *flag, int cycle) {
    //printf("enter emit_inst\n");
    if (*pc >= num_inst) {
        for (int i = 0; i < 7; ++i) {
            if (funcUnit[i].cyclesLeft >= 0)
                return true;
        }
        return false;
    }
    if (there_is_jump()) {
        return true;
    }
    if (*flag) {
        *pc += 1;
        if (*pc >= num_inst)
            return true;
        *flag = false;
    }
    if (instruction[*pc].op == ADD || instruction[*pc].op == SUB || instruction[*pc].op == JUMP) {
        for (int i = 0; i < 6; ++i) {
            if (resStation[i].busy)
                continue;
            resStation[i].busy = true;
            resStation[i].op = instruction[*pc].op;
            if (regStat[instruction[*pc].reg[1]].source == -1) {
                resStation[i].vj = regStat[instruction[*pc].reg[1]].value;
            } else {
                resStation[i].qj = regStat[instruction[*pc].reg[1]].source;
            }
            if (instruction[*pc].op == ADD || instruction[*pc].op == SUB) {
                if (regStat[instruction[*pc].reg[2]].source == -1) {
                    resStation[i].vk = regStat[instruction[*pc].reg[2]].value;
                } else {
                    resStation[i].qk = regStat[instruction[*pc].reg[2]].source;
                }
                regStat[instruction[*pc].reg[0]].source = i;  // set regStat
            }
            resStation[i].emit_cycle = cycle;
            if (resStation[i].qj == -1 && resStation[i].qk == -1)
                resStation[i].ready_cycle = cycle;
            resStation[i].inst_cnt = *pc;
            instruction[*pc].rsPos = i;
            if (instruction[*pc].t_emit == -1)
                instruction[*pc].t_emit = cycle;
            *flag = true;
            rs_to_fu(i, cycle);
            break;
        }
    } else if (instruction[*pc].op == MULT || instruction[*pc].op == DIV) {
        for (int i = 6; i < 9; ++i) {
            if (resStation[i].busy)
                continue;
            resStation[i].busy = true;
            resStation[i].op = instruction[*pc].op;
            if (regStat[instruction[*pc].reg[1]].source == -1) {
                resStation[i].vj = regStat[instruction[*pc].reg[1]].value;
            } else {
                resStation[i].qj = regStat[instruction[*pc].reg[1]].source;
            }
            if (regStat[instruction[*pc].reg[2]].source == -1) {
                resStation[i].vk = regStat[instruction[*pc].reg[2]].value;
            } else {
                resStation[i].qk = regStat[instruction[*pc].reg[2]].source;
            }
            resStation[i].emit_cycle = cycle;
            if (resStation[i].qj == -1 && resStation[i].qk == -1)
                resStation[i].ready_cycle = cycle;
            resStation[i].inst_cnt = *pc;
            instruction[*pc].rsPos = i;
            if (instruction[*pc].t_emit == -1)
                instruction[*pc].t_emit = cycle;
            regStat[instruction[*pc].reg[0]].source = i;  // set regStat
            *flag = true;
            rs_to_fu(i, cycle);
            break;
        }
    } else {  // LD
        for (int i = 0; i < 3; ++i) {
            if (loadBuffer[i].busy)
                continue;
            loadBuffer[i].busy = true;
            loadBuffer[i].address = instruction[*pc].reg[1];
            loadBuffer[i].emit_cycle = cycle;
            loadBuffer[i].ready_cycle = cycle;
            loadBuffer[i].inst_cnt = *pc;

            instruction[*pc].rsPos = i + 9;
            if (instruction[*pc].t_emit == -1)
                instruction[*pc].t_emit = cycle;
            regStat[instruction[*pc].reg[0]].source = i + 9;  // set regStat
            *flag = true;
            rs_to_fu(i + 9, cycle);
            break;
        }
    }

    return true;
}


