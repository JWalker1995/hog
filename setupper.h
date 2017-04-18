#ifndef SETUPPER_H
#define SETUPPER_H

#include <string>

#include "params.h"

typedef struct uv_loop_s uv_loop_t;
typedef struct uv_fs_s uv_fs_t;

class Setupper
{
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

    Setupper(const Params &params, uv_loop_t *loop);
    ~Setupper();

    const std::string &get_orig_dir() const {
        return orig_dir;
    }
    const std::string &get_bkp_dir() const {
        return bkp_dir;
    }

private:
    const Params &params;
    uv_loop_t *loop;

    std::string orig_dir;
    std::string bkp_dir;
};

#endif // SETUPPER_H
