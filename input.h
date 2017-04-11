#ifndef INPUT_H
#define INPUT_H

#include <string>

class Scheduler;

class Input
{
public:
    Input(Scheduler &scheduler);

    void read_data(const std::string &data);

    void process_param(const std::string &key, const std::string &val);
    void process_command(const std::string &cmd);

private:
    enum class ReadState {LineBegin, Comment, ParamKey, ParamVal, Command, Escape};
    ReadState read_state;
    std::string key_buf;
    std::string val_buf;

    Scheduler &scheduler;
};

#endif // INPUT_H
