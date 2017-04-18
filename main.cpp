#include <iostream>

#include "manager.h"

int main(int argc, char **argv) {
    Manager manager("mnt/");
    manager.run();

    return 0;
}
