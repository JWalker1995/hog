#include "manager.h"

#include "cwrapper.h"
#include "fs.h"

#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include <uv.h>

#include <iostream>

Manager::Manager()
    : fs(root_fd)
{
    loop = new uv_loop_t();
    uv_loop_init(loop);

    uv_poll_t fs_handle;
    uv_poll_init(loop, &fs_handle, fs.get_fd());
    fs_handle.data = this;
    uv_poll_start(&fs_handle, UV_READABLE, &fs_handle_callback);
}

void Manager::run() {
    uv_run(loop, UV_RUN_DEFAULT);
}

void Manager::~Manager() {
    uv_loop_close(loop);
    delete loop;
    return;


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

void Manager::fs_handle_callback(uv_poll_t *handle, int status, int events) {
    (void) events;

    if (status < 0) {
        std::cerr << uv_err_name(status) << ": " << uv_strerror(status) << std::endl;
        return;
    }

    get_instance(handle)->fs.try_recv();
}

Manager *Manager::get_instance(uv_poll_t *handle) {
    return static_cast<Manager *>(handle->data);
}
