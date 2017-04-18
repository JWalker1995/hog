#ifndef HOG_MANAGER_H
#define HOG_MANAGER_H

#include <string>

#include "params.h"
#include "setupper.h"
#include "fs.h"

typedef struct uv_loop_s uv_loop_t;

class Manager
{
public:
    Manager(const Params &params);
    ~Manager();

    void run();

private:
    uv_loop_t *loop;
    Setupper setupper;
    Fs fs;

    static uv_loop_t *create_loop();

    void fs_init_callback();
};

#endif // HOG_MANAGER_H
