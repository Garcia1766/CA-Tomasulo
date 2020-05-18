//
// Created by garcia on 5/12/20.
//

#include <string>

using namespace std;

string get_rs_name(int i) {
    switch (i) {
        case 0:
            return "Ars1";
        case 1:
            return "Ars2";
        case 2:
            return "Ars3";
        case 3:
            return "Ars4";
        case 4:
            return "Ars5";
        case 5:
            return "Ars6";
        case 6:
            return "Mrs1";
        case 7:
            return "Mrs2";
        case 8:
            return "Mrs3";
        default:
            return "    ";
    }
}

string get_lb_name(int i) {
    switch (i) {
        case 0:
            return "LB1 ";
        case 1:
            return "LB2 ";
        case 2:
            return "LB3 ";
        default:
            return "    ";
    }
}

string get_rss_name(int i) {
    return i < 9 ? get_rs_name(i) : get_lb_name(i - 9);
}

string get_reg_name(int i) {
    string ans = "R";
    return ans.append(to_string(i));
}

string get_fu_name(int i) {
    switch (i) {
        case 0:
            return "Add1";
        case 1:
            return "Add2";
        case 2:
            return "Add3";
        case 3:
            return "Mult1";
        case 4:
            return "Mult2";
        case 5:
            return "Load1";
        case 6:
            return "Load2";
        default:
            return "    ";
    }
}

string get_op(int i) {
    switch (i) {
        case 1:
            return "ADD ";
        case 2:
            return "SUB ";
        case 3:
            return "MULT";
        case 4:
            return "DIV ";
        case 5:
            return "LD  ";
        case 6:
            return "JUMP";
        default:
            return "    ";
    }
}
