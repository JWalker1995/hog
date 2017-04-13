#ifndef HOG_PROCESS_H
#define HOG_PROCESS_H

#include "processenv.h"

#include <string>

class Process
{
public:
    Process(const ProcessEnv &env, const std::string &cmd);

private:
    int pid;
};

#endif // HOG_PROCESS_H
