#ifndef PROCESSENV_H
#define PROCESSENV_H

#include <string>

struct ProcessEnv {
    std::string shell;
    const char *root_path;
};

#endif // PROCESSENV_H
