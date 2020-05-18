#include <iostream>
#include <string>
#include <ctime>
#include "Instruction.h"
#include "Tomasulo.h"

using namespace std;

int inst_num(string &test_name) {
    if (test_name == "0.basic")
        return 6;
    else if (test_name == "1.basic")
        return 34;
    else if (test_name == "2.basic")
        return 27;
    else if (test_name == "3.basic")
        return 28;
    else if (test_name == "4.basic")
        return 24;
    else if (test_name == "Fact")
        return 10;
    else if (test_name == "Fabo")
        return 14;
    else if (test_name == "Example")
        return 8;
    else if (test_name == "Gcd")
        return 21;
    else if (test_name == "Mul")
        return 1018;
    else if (test_name == "Big_test")
        return 1000000;
    else if (test_name == "my")
        return 5;
    else
        return 1;
}

int read_hex(char *buf, int start) {
    int ans = 0;
    for (int i = start; isdigit(buf[i]) || 'A' <= buf[i] && buf[i] <= 'F'; ++i) {
        if ('A' <= buf[i] && buf[i] <= 'F') {
            ans = (ans << 4) + buf[i] - 'A' + 10;
        } else {
            ans = (ans << 4) + buf[i] - '0';
        }
    }
    return ans;
}

void read_instruction(Instruction &instruction, FILE *input_file) {
    char buf[64];
    int reg = 0;
    fgets(buf, 64, input_file);
    if (buf[0] == 'A' || buf[0] == 'S' || buf[0] == 'M' || buf[0] == 'D') {
        switch (buf[0]) {
            case 'A':
                instruction.op = ADD;
                break;
            case 'S':
                instruction.op = SUB;
                break;
            case 'M':
                instruction.op = MULT;
                break;
            case 'D':
                instruction.op = DIV;
                break;
            default:
                break;
        }
        int i = 0;
        for (; buf[i] != ','; ++i) {}
        i += 2;
        for (; buf[i] != ','; ++i) {
            reg = reg * 10 + buf[i] - '0';
        }
        instruction.reg[0] = reg;
        reg = 0;
        i += 2;
        for (; buf[i] != ','; ++i) {
            reg = reg * 10 + buf[i] - '0';
        }
        instruction.reg[1] = reg;
        reg = 0;
        i += 2;
        for (; isdigit(buf[i]); ++i) {
            reg = reg * 10 + buf[i] - '0';
        }
        instruction.reg[2] = reg;
    } else if (buf[0] == 'L') {
        instruction.op = LD;
        int i = 4;
        for (; buf[i] != ','; ++i) {
            reg = reg * 10 + buf[i] - '0';
        }
        instruction.reg[0] = reg;
        instruction.reg[1] = read_hex(buf + i + 3, 0);
    } else {
        instruction.op = JUMP;
        int i = 0;
        for (; buf[i] != ','; ++i) {}
        instruction.reg[0] = read_hex(buf + i + 3, 0);
        i += 3;
        for (; buf[i] != ','; ++i) {}
        i += 2;
        for (; buf[i] != ','; ++i) {
            reg = reg * 10 + buf[i] - '0';
        }
        instruction.reg[1] = reg;
        instruction.reg[2] = read_hex(buf + i + 3, 0);
    }
}

void print_state(Tomasulo &tomasulo, string test_name, int cycle) {
    string output_name = "../states/";
    output_name.append(test_name).append("_").append(to_string(cycle));
    FILE *output_file = fopen(output_name.data(), "w");
    fprintf(output_file, "\t\tBusy\tOp\t\tVj\t\tVk\t\tQj\t\tQk\n");
    for (int i = 0; i < 9; ++i) {
        fprintf(output_file, "%s\t%s\t\t%s\t%-8d%-8d%s\t%s\n", get_rs_name(i).data(), tomasulo.resStation[i].busy ? "Yes" : "No",
                get_op(tomasulo.resStation[i].op).data(), tomasulo.resStation[i].vj, tomasulo.resStation[i].vk,
                get_rss_name(tomasulo.resStation[i].qj).data(), get_rss_name(tomasulo.resStation[i].qk).data());
    }
    fprintf(output_file, "\n\t\tBusy\tAddr\n");
    for (int i = 0; i < 3; ++i) {
        fprintf(output_file, "%s\t%s\t\t%-12x\n", get_lb_name(i).data(), tomasulo.loadBuffer[i].busy ? "Yes" : "No",
                tomasulo.loadBuffer[i].address);
    }

    fprintf(output_file, "\n\t\t");
    for (int i = 0; i < 32; ++i) {
        fprintf(output_file, "%s\t\t", get_reg_name(i).data());
    }
    fprintf(output_file, "\nvalue\t");
    for (int i = 0; i < 32; ++i) {
        fprintf(output_file, "%d\t\t", tomasulo.regStat[i].value);
    }
    fprintf(output_file, "\nstate\t");
    for (int i = 0; i < 32; ++i) {
        fprintf(output_file, "%s\t", get_rss_name(tomasulo.regStat[i].source).data());
    }
    fprintf(output_file, "\n\n");

    fprintf(output_file, "\t\tOp\t\tCycle\n");
    for (int i = 0; i < 7; ++i) {
        fprintf(output_file, "%s\t%s\t%d\n", get_fu_name(i).data(), get_op(tomasulo.funcUnit[i].op).data(),
                tomasulo.funcUnit[i].cyclesLeft);
    }
    fclose(output_file);
}

int main(int argc, char *argv[]) {
    string test_name = "../TestCase/";
    test_name.append(argv[1]).append(".nel");
    int num_inst = inst_num(*(new string(argv[1])));

    clock_t t0 = clock();
    Instruction *instruction = new Instruction[num_inst];
    FILE *input_file = fopen(test_name.data(), "r");
    for (int i = 0; i < num_inst; ++i) {
        read_instruction(instruction[i], input_file);
    }
    fclose(input_file);

    Tomasulo tomasulo(instruction, num_inst);
    int pc = 0;
    bool flag = false;
    int cycle = 1;
    bool loop = true;
    clock_t t1 = clock();
    for (; loop; cycle++) {
        tomasulo.decrease_fu(&pc, &flag, cycle);
        loop = tomasulo.emit_inst(&pc, &flag, cycle);
        if (argv[1][0] != 'B' && argv[1][0] != 'G' && argv[1][0] != 'M')
            print_state(tomasulo, argv[1], cycle);
    }
    printf("execution time: %fms\n", (double)(clock() - t1) / CLOCKS_PER_SEC * 1000);
    printf("finish in %d cycles\n", cycle - 1);

    string log_name = "../Log/2017011235_";
    log_name.append(argv[1]).append(".log");
    FILE *log_file = fopen(log_name.data(), "w");
    for (int i = 0; i < num_inst; ++i) {
        fprintf(log_file, "%d %d %d\n", instruction[i].t_emit, instruction[i].t_ex, instruction[i].t_wb);
    }
    fclose(log_file);
    printf("total time cost: %fms\n", (double)(clock() - t0) / CLOCKS_PER_SEC * 1000);

    return 0;
}
