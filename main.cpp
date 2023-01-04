#include <iostream>
#include <stdio.h>
#include <string.h>

int main() {
    FILE* fp = popen("xdotool search --name \"AssaultCube\" getwindowpid", "r");
    if (fp == NULL) {
        std::cerr << "Failed to run xdotool" << std::endl;
        return 1;
    }

    char output[10];
    fgets(output, sizeof(output)-1, fp);
    pclose(fp);

    std::cout << "PID: " << output << std::endl;
    return 0;
}
