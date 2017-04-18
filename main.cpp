#include <iostream>

#include "manager.h"

int main(int argc, char **argv) {
    Params params(argc, argv);

    Manager manager(params);
    manager.run();

    return 0;
}
