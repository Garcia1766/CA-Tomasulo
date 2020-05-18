//
// Created by garcia on 5/10/20.
//

#ifndef TOMASULO_UTIL_H
#define TOMASULO_UTIL_H

#include <string>

using namespace std;

#define ADD 1
#define SUB 2
#define MULT 3
#define DIV 4
#define LD 5
#define JUMP 6

extern string get_rs_name(int);
extern string get_lb_name(int);
extern string get_rss_name(int);
extern string get_reg_name(int);
extern string get_fu_name(int);
extern string get_op(int);

#endif //TOMASULO_UTIL_H
