#ifndef CWRAPPER_H
#define CWRAPPER_H

#include "jw_util/to_string.h"

#include <string>

class CWrapper {
public:
    class Exception : public std::exception {
    public:
        Exception()
        {}

        virtual const char *what() const noexcept {
            return "CWrapper::Exception";
        }
    };

    template <typename FuncType, typename... ArgTypes>
    static auto call(const std::string &func_name, FuncType func, ArgTypes... args) {
        auto res = func(args...);
        handle_error<ArgTypes...>(res, func_name, args...);
        return res;
    }

    template <typename... ArgTypes>
    static void handle_error(signed int res, const std::string &func_name, ArgTypes... args) {
        if (res < 0) {
            std::string msg = func_name + "(" + args_to_string(args...) + ") -> " + std::to_string(res);
            perror(msg.c_str());
            throw Exception();
        }
    }
    template <typename... ArgTypes>
    static void handle_error(void *res, const std::string &func_name, ArgTypes... args) {
        if (!res) {
            std::string msg = func_name + "(" + args_to_string(args...) + ") -> (void*)NULL";
            perror(msg.c_str());
            throw Exception();
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
};

#endif // CWRAPPER_H
