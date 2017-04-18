#ifndef SETUPPER_H
#define SETUPPER_H

#include <string>

typedef struct uv_loop_s uv_loop_t;
typedef struct uv_fs_s uv_fs_t;

class Setupper
{
public:
    Setupper(uv_loop_t *loop, const std::string &source_path);

private:
    uv_loop_t *loop;
    const std::string &source_path;

    static Setupper *get_instance(uv_fs_t *req);
};

#endif // SETUPPER_H
