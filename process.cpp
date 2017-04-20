#include "process.h"

#include "cwrapper.h"

#include <unistd.h>

static void copy_string(char *dst, const std::string &src) {
    std::size_t size = src.size() + 1;
    dst = new char[size];
    for (std::size_t i = 0; i < size; i++) {
        dst[i] = src.c_str()[i];
    }
}

Process::Process(const ProcessEnv &env, const std::string &cmd) {
    pid_t child_pid = fork();

    if (child_pid) {
        // Parent
        pid = child_pid;
    } else {
        // Child

        char *argv[4];
        copy_string(argv[0], env.shell);
        copy_string(argv[1], "-c");
        copy_string(argv[2], cmd);
        argv[3] = 0;

        char *const envp[] = {
            (char *) 0,
        };

        /*
        CWrapper::call("chroot", &chroot, env.root_path);
        CWrapper::call("chdir", &chdir, "/");
        */

        CWrapper::call("execve", &execve, env.shell.c_str(), argv, envp);
    }
}
