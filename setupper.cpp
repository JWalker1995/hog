#include "setupper.h"

#include <uv.h>

#include <iostream>

Setupper::Setupper(uv_loop_t *loop, const std::string &source_path)
    : loop(loop)
    , source_path(source_path)
{
    uv_fs_t req;

    std::string bkp_path;
    unsigned int index = 0;
    int mkdir_res;
    do {
        // This is the path to backup the code to
        bkp_path = source_path + "/../.hog_" + std::to_string(index);
        index++;

        // This does lots of things:
        // Tries to create a directory
        // If fails (because one exists already), try another name
        // The new directory will immediately be overwritten by the renameat call,
        //   but it's a good to make the test/create step an atomic operation
        int res = uv_fs_mkdir(loop, &req, bkp_path.c_str(), 0700, 0);
        std::cout << res << std::endl;
        break;

        /*
        mkdir_res = mkdirat(parent_fd, bkp_path.c_str(), 0700);
        int uv_fs_stat(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb)
        */
    } while (mkdir_res < 0 && errno == EEXIST);
/*
    uv_fs_rename(loop, &req, source_path, bkp_path, uv_fs_cb cb);

    const char *abs_root_path = realpath(".", 0);
    int root_fd = CWrapper::call("open", open, abs_root_path, O_DIRECTORY);
    int parent_fd = CWrapper::call("openat", openat, root_fd, "..", O_DIRECTORY);

    std::string bkp_path;
    unsigned int index = 0;
    int mkdir_res;
    do {
        // This is the path to backup the code to
        bkp_path = ".hog_" + std::to_string(index);
        index++;

        // This does lots of things:
        // Tries to create a directory
        // If fails (because one exists already), try another name
        // The new directory will immediately be overwritten by the renameat call,
        //   but it's a good to make the test/create step an atomic operation
        mkdir_res = mkdirat(parent_fd, bkp_path.c_str(), 0700);
    } while (mkdir_res < 0 && errno == EEXIST);

    CWrapper::handle_error(mkdir_res, "mkdir", parent_fd, bkp_path.c_str());

    CWrapper::call("renameat", renameat, AT_FDCWD, abs_root_path, parent_fd, bkp_path.c_str());

    CWrapper::call("mkdir", mkdir, abs_root_path, 0700);

    Fs fs(root_fd);
    fs.set_init_callback(jw_util::MethodCallback<>::create<Manager, &Manager::fs_init_callback>(this));
    fs.start(abs_root_path);

    CWrapper::call("renameat", renameat, parent_fd, bkp_path.c_str(), AT_FDCWD, abs_root_path);

    std::free(const_cast<char *>(abs_root_path));
    */
}

Setupper *Setupper::get_instance(uv_fs_t *req) {
    return static_cast<Setupper *>(req->data);
}
