#include "manager.h"

#include "cwrapper.h"
#include "fs.h"

#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include <iostream>

Manager::Manager() {
}

void Manager::run() {
    const char *abs_root_path = realpath(".", 0);
    int root_fd = CWrapper::call("open", open, abs_root_path, O_DIRECTORY);
    int parent_fd = CWrapper::call("openat", openat, root_fd, "..", O_DIRECTORY);

    std::string bkp_dir;
    unsigned int index = 0;
    int mkdir_res;
    do {
        // This is the path to backup the code to
        bkp_dir = ".hog_" + std::to_string(index);
        index++;

        // This does lots of things:
        // Tries to create a directory
        // If fails (because one exists already), try another name
        // The new directory will immediately be overwritten by the renameat call,
        //   but it's a good to make the test/create step an atomic operation
        mkdir_res = mkdirat(parent_fd, bkp_dir.c_str(), 0700);
    } while (mkdir_res < 0 && errno == EEXIST);

    CWrapper::handle_error(mkdir_res, "mkdir", parent_fd, bkp_dir.c_str());

    CWrapper::call("renameat", renameat, AT_FDCWD, abs_root_path, parent_fd, bkp_dir.c_str());

    CWrapper::call("mkdir", mkdir, abs_root_path, 0700);

    Fs fs(root_fd);
    fs.set_init_callback(jw_util::MethodCallback<>::create<Manager, &Manager::fs_init_callback>(this));
    fs.start(abs_root_path);

    CWrapper::call("renameat", renameat, parent_fd, bkp_dir.c_str(), AT_FDCWD, abs_root_path);

    std::free(const_cast<char *>(abs_root_path));
}

void Manager::fs_init_callback() {
    // Kickoff processes
    std::cout << "Init callback" << std::endl;
}
