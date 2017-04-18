#ifndef HOG_MANAGER_H
#define HOG_MANAGER_H

#include <string>

#include "fs.h"

class uv_loop_t;
class uv_poll_t;

class Manager
{
public:
    Manager();

    void run();

    ~Manager();

private:
    Fs fs;
    uv_loop_t *loop;

    void fs_init_callback();

    static void fs_handle_callback(uv_poll_t* handle, int status, int events);

    static Manager *get_instance(uv_poll_t *handle);
};

#endif // HOG_MANAGER_H
