/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello_ll.c `pkg-config fuse --cflags --libs` -o hello_ll
*/

#include "fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "cwrapper.h"

#include <iostream>

static const char *hello_str = "Hello World!\n";
static const char *hello_name = "hello";

static int hello_stat(fuse_ino_t ino, struct stat *stbuf)
{
    std::cout << "stat " << ino << std::endl;

    stbuf->st_ino = ino;
    switch (ino) {
    case 1:
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        break;

    case 2:
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_str);
        break;

    default:
        return -1;
    }
    return 0;
}

void Fs::hook_init(void *userdata, fuse_conn_info *conn) {
    (void) conn;
    get_instance(userdata)->init_callback.call();
}

void Fs::hook_getattr(fuse_req_t req, fuse_ino_t ino,
                 struct fuse_file_info *fi)
{
    std::cout << "getattr " << ino << std::endl;

    struct stat stbuf;

    (void) fi;

    memset(&stbuf, 0, sizeof(stbuf));
    if (hello_stat(ino, &stbuf) == -1)
        fuse_reply_err(req, ENOENT);
    else
        fuse_reply_attr(req, &stbuf, 1.0);
}

void Fs::hook_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
    /*
    Fs *inst = get_instance(req);
    try {
        struct fuse_entry_param entry;
        memset(&entry, 0, sizeof(entry));
        entry.ino = inst->tree.get_node(parent)->lookup(name);
        entry.attr_timeout = 1.0;
        entry.entry_timeout = 1.0;
        memcpy(entry.attr, )
        apply_stat(entry.attr, entry.ino);

    } catch (const VirtualTree::InodeException &ex) {
        fuse_reply_err(req, ENOENT);
    } catch (const VirtualNode::LookupException &ex) {
        fuse_reply_err(req, ENOENT);
    }
*/
    std::cout << "lookup " << name << std::endl;



    if (parent != 1 || strcmp(name, hello_name) != 0)
        fuse_reply_err(req, ENOENT);
    else {
        memset(&e, 0, sizeof(e));
        e.ino = 2;
        e.attr_timeout = 1.0;
        e.entry_timeout = 1.0;
        hello_stat(e.ino, &e.attr);

        fuse_reply_entry(req, &e);
    }
}

struct dirbuf {
    char *p;
    size_t size;
};

static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name,
               fuse_ino_t ino)
{
    struct stat stbuf;
    size_t oldsize = b->size;
    b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
    b->p = (char *) realloc(b->p, b->size);
    memset(&stbuf, 0, sizeof(stbuf));
    stbuf.st_ino = ino;
    fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf,
              b->size);
}

#define min(x, y) ((x) < (y) ? (x) : (y))

static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize,
                 off_t off, size_t maxsize)
{
    if (off < bufsize)
        return fuse_reply_buf(req, buf + off,
                      min(bufsize - off, maxsize));
    else
        return fuse_reply_buf(req, NULL, 0);
}

void Fs::hook_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
                 off_t off, struct fuse_file_info *fi)
{
    std::cout << "readdir " << ino << std::endl;

    (void) fi;

    if (ino != 1)
        fuse_reply_err(req, ENOTDIR);
    else {
        struct dirbuf b;

        memset(&b, 0, sizeof(b));
        dirbuf_add(req, &b, ".", 1);
        dirbuf_add(req, &b, "..", 1);
        dirbuf_add(req, &b, hello_name, 2);
        reply_buf_limited(req, b.p, b.size, off, size);
        free(b.p);
    }
}

void Fs::hook_open(fuse_req_t req, fuse_ino_t ino,
              struct fuse_file_info *fi)
{
    std::cout << "open " << ino << std::endl;

    if (ino != 2)
        fuse_reply_err(req, EISDIR);
    else if ((fi->flags & 3) != O_RDONLY)
        fuse_reply_err(req, EACCES);
    else
        fuse_reply_open(req, fi);
}

void Fs::hook_read(fuse_req_t req, fuse_ino_t ino, size_t size,
              off_t off, struct fuse_file_info *fi)
{
    std::cout << "read " << ino << std::endl;

    (void) fi;

    assert(ino == 2);
    reply_buf_limited(req, hello_str, strlen(hello_str), off, size);
}

void Fs::hook_write(fuse_req_t req, fuse_ino_t ino, const char *buf,
                           size_t size, off_t off, struct fuse_file_info *fi) {
}

Fs::Fs(int source_fd, const char *mountpoint)
    //: source_root(source_fd)
    : init_callback(jw_util::MethodCallback<>::create_dummy())
{
    char *argv[] = {
        (char *) "fuse_bindings_dummy"
    };
    struct fuse_args args = FUSE_ARGS_INIT(1, argv);

    ch = CWrapper::call("fuse_mount", fuse_mount, mountpoint, &args);
    se = CWrapper::call("fuse_lowlevel_new", fuse_lowlevel_new, &args, &ops, sizeof(ops), this);
    CWrapper::call("fuse_set_signal_handlers", fuse_set_signal_handlers, se);
    fuse_session_add_chan(se, ch);

    size_t bufsize = fuse_chan_bufsize(ch);
    recv_buf = new char[bufsize];

    int flags = CWrapper::call("fcntl", fcntl, get_fd(), F_GETFL, 0);
    CWrapper::call("fcntl", fcntl, get_fd(), F_SETFL, flags | O_NONBLOCK);
}

int Fs::get_fd() const {
    return fuse_chan_fd(ch);
}

bool Fs::should_exit() const {
    return fuse_session_exited(se);
}

void Fs::try_recv() {
    struct fuse_buf fbuf = {
        .mem = recv_buf,
        .size = bufsize,
    };

    int res = fuse_session_receive_buf(se, &fbuf, &ch);

    if (res == -EINTR) {return;}
    if (res == -EAGAIN) {return;}
    if (res < 0) {
        CWrapper::handle_error(res, "fuse_session_receive_buf", se, &recv_buf, &tmpch);
    }

    fuse_session_process_buf(se, &fbuf, ch);
}

void Fs::~Fs() {
    delete[] recv_buf;

    fuse_session_reset(se);

    fuse_remove_signal_handlers(se);
    fuse_session_remove_chan(ch);
    fuse_session_destroy(se);
    fuse_unmount(mountpoint, ch);
}
