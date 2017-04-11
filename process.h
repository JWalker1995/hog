#ifndef PROCESS_H
#define PROCESS_H

#include "processenv.h"

#include <string>

class Process
{
public:
    Process(const ProcessEnv &env, const std::string &cmd);

private:
    int pid;
};

#endif // PROCESS_H
