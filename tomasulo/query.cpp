//
// Created by garcia on 5/18/20.
//

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    string state_filename = "../states/";
    state_filename.append(argv[1]).append("_").append(argv[2]);
    FILE *input_file = fopen(state_filename.data(), "r");
    for (; !feof(input_file);) {
        char buf[1024];
        fgets(buf, 1024, input_file);
        printf("%s", buf);
        //puts(buf);
    }
    fclose(input_file);

    return 0;
}
