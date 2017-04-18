#include "virtualtree.h"

#include "cwrapper.h"

#include <assert.h>
#include <dirent.h>
#include <errno.h>


void VirtualTree::load_dir(VirtualNode &node) {
    DIR *dir = CWrapper::call("fdopendir", fdopendir, node.self_fd);

    errno = 0;
    struct dirent *ent;
    while ((ent = readdir(dir))) {
        Type child_type;
        switch (ent->d_type) {
        case DT_BLK:
        case DT_CHR:
        case DT_FIFO:
        case DT_LNK:
        case DT_SOCK:
            assert(false);
            break;

        case DT_REG:
            child_type = Type::File;
            break;

        case DT_DIR:
            child_type = Type::Directory;
            break;

        case DT_UNKNOWN:
        default:
            child_type = Type::Unknown;
        }

        VirtualNode &child = pool.empla();
        child.parent_fd = self_fd;
        child.self_name = ent->d_name;
        child.self_fd = 0;
        child.type = child_type;
        children.push_back(&child);
    }

    if (errno) {
        CWrapper::handle_error((void *) 0, "readdir", ent);
    }

    closedir(dir);
}
