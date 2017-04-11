#include "input.h"

#include "scheduler.h"

#include <unistd.h>

Input::Input(Scheduler &scheduler)
    : scheduler(scheduler)
{
}

void Input::read_data(const std::string &data) {
    std::string::const_iterator i = data.cbegin();
    while (i != data.cend()) {
        char c = *i;

        switch (read_state) {
        case ReadState::LineBegin:
            if (c == ':') {
                read_state = ReadState::ParamKey;
            } else if (c == '#') {
                read_state = ReadState::Comment;
            } else {
                val_buf += c;
            }
            break;

        case ReadState::Comment:
            if (c == '\n') {
                read_state = ReadState::LineBegin;
            }
            break;

        case ReadState::ParamKey:
            if (c == ' ') {
                read_state = ReadState::ParamVal;
            } else if (c == '\n') {
                process_param(key_buf, val_buf);
                key_buf.clear();
                read_state = ReadState::LineBegin;
            } else {
                key_buf += c;
            }
            break;

        case ReadState::ParamVal:
            if (c == '\n') {
                process_param(key_buf, val_buf);
                key_buf.clear();
                val_buf.clear();
                read_state = ReadState::LineBegin;
            } else {
                val_buf += c;
            }
            break;

        case ReadState::Command:
            if (c == '\\') {
                read_state = ReadState::Escape;
            } else if (c == '\n') {
                process_command(val_buf);
                val_buf.clear();
                read_state = ReadState::LineBegin;
            } else {
                val_buf += c;
            }
            break;

        case ReadState::Escape:
            val_buf += c;
            read_state = ReadState::Command;
            break;
        }

        i++;
    }
}

void Input::process_param(const std::string &key, const std::string &val) {

}

void Input::process_command(const std::string &cmd) {
    scheduler.schedule_command(cmd);
}
