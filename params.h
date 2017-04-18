#ifndef PARAMS_H
#define PARAMS_H

#include <string>

class Params {
public:
    class Exception : public std::exception {
    public:
        Exception(const std::string &msg)
            : msg(msg)
        {}

        virtual const char *what() const noexcept {
            return msg.c_str();
        }

    private:
        std::string msg;
    };

    Params(unsigned int argc, char **argv);

    const std::string &get_target_dir() const {
        return target_dir;
    }
    const std::string &get_config_filename() const {
        return config_filename;
    }

private:
    std::string target_dir = ".";
    std::string config_filename = "hog.hog";
};

#endif // PARAMS_H
