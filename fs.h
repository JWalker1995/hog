#ifndef HOG_FS_H
#define HOG_FS_H

#include <string>
#include <exception>

#include "include_fuse.h"
#include "jw_util/methodcallback.h"

class Fs {
public:
    class Exception : public std::exception {
    public:
        Exception(const std::string &msg)
            : msg(msg)
        {}

        virtual const char *what() const noexcept {
            return msg.c_str();
        }

    private:
        std::string msg;
    };

    Fs(int source_fd, const char *mountpoint);

    void set_init_callback(jw_util::MethodCallback<> callback) {
        init_callback = callback;
    }

    int get_fd() const;
    bool should_exit() const;
    void try_recv();

    ~Fs();

private:
    static void hook_init(void *userdata, struct fuse_conn_info *conn);
    static void hook_getattr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
    static void hook_lookup(fuse_req_t req, fuse_ino_t parent, const char *name);
    static void hook_readdir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);
    static void hook_open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
    static void hook_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);
    static void hook_write(fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size, off_t off, struct fuse_file_info *fi);

    static fuse_lowlevel_ops make_ops();

    static Fs *get_instance(fuse_req_t req) {
        return get_instance(fuse_req_userdata(req));
    }
    static Fs *get_instance(void *ptr) {
        return static_cast<Fs *>(ptr);
    }

    const char *mountpoint;
    struct fuse_chan *ch;
    struct fuse_session *se;
    char *recv_buf_mem;
    std::size_t recv_buf_size;

    //VirtualNode source_root;
    jw_util::MethodCallback<> init_callback;

    fuse_lowlevel_ops ops = {
        .init       = Fs::hook_init,
        .lookup     = Fs::hook_lookup,
        .getattr	= Fs::hook_getattr,
        .readdir	= Fs::hook_readdir,
        .open		= Fs::hook_open,
        .read		= Fs::hook_read,
    };
};

#endif // HOG_FS_H
