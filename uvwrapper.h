#ifndef UVWRAPPER_H
#define UVWRAPPER_H

#include "jw_util/to_string.h"

#include <string>

#include <uv.h>

class UvWrapper {
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

    template <typename... ArgTypes>
    static signed int call(const std::string &func_name, signed int (* func)(ArgTypes...), ArgTypes... args) {
        signed int res = func(args...);
        handle<ArgTypes...>(res, func_name, args...);
        return res;
    }

    template <typename... ArgTypes>
    static void handle(signed int res, const std::string &func_name, ArgTypes... args) {
        if (res < 0) {
            std::string msg = func_name + "(" + args_to_string(args...) + ") -> " + std::to_string(res) + " " + uv_err_name(res) + " " + uv_strerror(res);
            throw Exception(msg);
        }
    }

private:
    template <typename FirstArgType, typename... ArgTypes>
    static std::string args_to_string(const FirstArgType &first_arg, const ArgTypes &... args) {
        return jw_util::to_string(first_arg) + ", " + args_to_string(args...);
    }

    template <typename ArgType>
    static std::string args_to_string(const ArgType &arg) {
        return jw_util::to_string(arg);
    }

    static std::string args_to_string() {
        return "";
    }
};

#endif // UVWRAPPER_H
