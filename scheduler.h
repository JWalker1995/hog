#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>

class Scheduler
{
public:
    Scheduler();

    void schedule_command(const std::string &cmd);

private:
    int proxy_file_desc;
};

#endif // SCHEDULER_H
