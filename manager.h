#ifndef HOG_MANAGER_H
#define HOG_MANAGER_H

#include <string>

#include "setupper.h"
#include "fs.h"

typedef struct uv_loop_s uv_loop_t;
typedef struct uv_poll_s uv_poll_t;

class Manager
{
public:
    Manager(const std::string &mountpoint);

    void run();

    ~Manager();

private:
    uv_loop_t *loop;
    Setupper setupper;
    Fs fs;

    void fs_init_callback();

    static void fs_handle_callback(uv_poll_t* handle, int status, int events);

    static Manager *get_instance(uv_poll_t *handle);
};

#endif // HOG_MANAGER_H
