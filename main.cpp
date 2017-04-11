#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "fs.h"
#include "string_util.h"

struct noop {
    void operator()(...) const {}
};

class Hog {
public:
    template <typename StreamType>
    void read_stream(StreamType &stream) {
        std::string line;
        while (std::getline(stream, line)) {
            string_util::trim(line);

            // Skip empty lines
            if (line.empty()) {continue;}

            // Skip comment lines
            if (line[0] == '#') {continue;}

            if (line[0] == ':') {
                // Parse directive lines

                unsigned int space = line.find(' ', 1);
                std::string key = line.substr(1, space);
                std::string val = line.substr(space + 1);
                directives.emplace(key, val);
            } else {
                // Parse command lines

                cmds.emplace_back(std::move(line));
            }
        }
    }

private:
    std::unordered_map<std::string, std::string> directives;

    std::vector<std::string> cmds;
};

int main(int argc, char **argv) {
    Hog hog;

    // Read from hog.hog
    {
        std::ifstream file("hog.hog");
        hog.read_stream<std::ifstream>(file);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-") == 0) {
            // Read from stdin
            hog.read_stream<std::istream>(std::cin);
        } else {
            // Read from command line argument
            std::ifstream file(argv[i]);
            hog.read_stream<std::ifstream>(file);
        }
    }

    bool succ = hog_fs("mnt");

    return !succ;
}
