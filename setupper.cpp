#include "setupper.h"

#include "uvwrapper.h"

#include <assert.h>

#include <uv.h>

#include <iostream>

Setupper::Setupper(const Params &params, uv_loop_t *loop)
    : params(params)
    , loop(loop)
{
    uv_fs_t req;
    void (*sync_callback)(uv_fs_t *) = 0;

    UvWrapper::call("uv_fs_realpath", uv_fs_realpath, loop, &req, params.get_target_dir().c_str(), sync_callback);
    std::string target_path = std::string(static_cast<char*>(req.ptr));

    if (target_path.back() != '/') {
        target_path.push_back('/');
    }

    std::string config_path;
    unsigned int orig_dir_size;

    int open_res;
    do {
        if (target_path.size() == 1) {
            assert(target_path == "/");
            throw Exception("Cannot find " + params.get_config_filename() + " in " + params.get_target_dir() + " or any parent directories");
        }

        config_path = target_path + params.get_config_filename();
        orig_dir_size = target_path.size();
        open_res = uv_fs_open(loop, &req, config_path.c_str(), O_RDONLY, 0, sync_callback);

        do {
            target_path.pop_back();
        } while (target_path.back() != '/');
    } while (open_res == UV_ENONET);

    UvWrapper::handle(open_res, "uv_fs_open", loop, &req, config_path.c_str(), O_RDONLY, 0, sync_callback);

    const std::string parent_dir = std::move(target_path);
    orig_dir = config_path.substr(0, orig_dir_size);

    unsigned int index = 0;
    int mkdir_res;
    do {
        // This is the path to backup the code to
        bkp_dir = parent_dir + ".hog_" + std::to_string(index);
        index++;

        // This does lots of things:
        // Tries to create a directory
        // If fails (because one exists already), try another name
        // The new directory will immediately be overwritten by the renameat call,
        //   but it's a good to make the test/create step an atomic operation
        mkdir_res = uv_fs_mkdir(loop, &req, bkp_dir.c_str(), 0700, sync_callback);
    } while (mkdir_res == UV_EEXIST);

    UvWrapper::handle(mkdir_res, "uv_fs_mkdir", loop, &req, bkp_dir.c_str(), 0700, sync_callback);

    UvWrapper::call("uv_fs_rename", uv_fs_rename, loop, &req, orig_dir.c_str(), bkp_dir.c_str(), sync_callback);

    UvWrapper::call("uv_fs_mkdir", uv_fs_mkdir, loop, &req, orig_dir.c_str(), 0700, sync_callback);
}

Setupper::~Setupper() {
    uv_fs_t req;
    void (*sync_callback)(uv_fs_t *) = 0;
    UvWrapper::call("uv_fs_rename", uv_fs_rename, loop, &req, bkp_dir.c_str(), orig_dir.c_str(), sync_callback);
}
