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

Manager::Manager(const std::string &mountpoint)
    : loop(new uv_loop_t())
    , setupper(loop, mountpoint)
    , fs(0, 0)
{
    uv_loop_init(loop);

    uv_poll_t fs_handle;
    uv_poll_init(loop, &fs_handle, fs.get_fd());
    fs_handle.data = this;
    uv_poll_start(&fs_handle, UV_READABLE, &fs_handle_callback);
}

void Manager::run() {
    uv_run(loop, UV_RUN_DEFAULT);
}

Manager::~Manager() {
    uv_loop_close(loop);
    delete loop;
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
