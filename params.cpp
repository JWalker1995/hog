#include "params.h"

#include <unordered_map>

Params::Params(unsigned int argc, char **argv) {
    // Config
    std::unordered_map<std::string, std::string *> flags;
    flags["arg_1"] = &target_dir;
    flags["--config_file"] = &config_filename;

    unsigned int next_arg = 1;
    std::string *next_dst = 0;
    for (unsigned int i = 1; i < argc; i++) {
        if (next_dst) {
            *next_dst = argv[i];
        } else if (argv[i][0] == '-') {
            const std::string flag_key = argv[i];
            std::unordered_map<std::string, std::string *>::const_iterator dst = flags.find(flag_key);
            if (dst == flags.cend()) {
                throw Exception("Invalid flag " + flag_key);
            }
            next_dst = dst->second;
        } else {
            const std::string flag_key = "arg_" + std::to_string(next_arg);
            std::unordered_map<std::string, std::string *>::const_iterator dst = flags.find(flag_key);
            if (dst == flags.cend()) {
                throw Exception("Too many arguments supplied");
            }
            next_arg++;
            *dst->second = argv[i];
        }
    }
}
