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

Manager::Manager(const Params &params)
    : loop(create_loop())
    , setupper(params, loop)
    , fs(params, loop, setupper.get_bkp_dir(), setupper.get_orig_dir())
{}

Manager::~Manager() {
    uv_loop_close(loop);
    delete loop;
}

void Manager::run() {
    uv_run(loop, UV_RUN_DEFAULT);
}

uv_loop_t *Manager::create_loop() {
    uv_loop_t *loop = new uv_loop_t();
    uv_loop_init(loop);
    return loop;
}

void Manager::fs_init_callback() {
    // Kickoff processes
    std::cout << "Init callback" << std::endl;
}
