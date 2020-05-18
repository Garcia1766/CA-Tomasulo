//
// Created by garcia on 5/10/20.
//

#ifndef TOMASULO_REGSTAT_H
#define TOMASULO_REGSTAT_H


class RegStat {
public:
    int value;
    int source;  // 0 <= result[i] <= 11, means RS/LB -> Ri

    RegStat();
};


#endif //TOMASULO_REGSTAT_H
