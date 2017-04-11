#ifndef CWRAPPER_H
#define CWRAPPER_H

#include "jw_util/to_string.h"

#include <string>

class CWrapper {
public:
    template <typename FuncType, typename... ArgTypes>
    static bool call(const std::string &func_name, FuncType func, ArgTypes... args) {
        signed int res = func(args...);
        if (res < 0) {
            std::string msg = func_name + "(" + args_to_string(args...) + ")";
            perror(msg.c_str());
            return false;
        }
        return true;
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
