#include "sourcenode.h"

#include "cwrapper.h"

#include <dirent.h>
#include <errno.h>

void SourceNode::load_dir() {
    DIR *dir = fdopendir(self_fd);
    if (dir) {
        errno = 0;
        struct dirent *ent;
        while (ent = readdir(dir)) {
            Type child_type;
            switch (ent->d_type) {
            case DT_BLK:
            case DT_CHR:
            case DT_FIFO:
            case DT_LNK: // TODO: ?
            case DT_REG:
            case DT_SOCK:
                child_type = Type::File;
                break;

            case DT_DIR:
                child_type = Type::Directory;
                break;

            case DT_UNKNOWN:
            default:
                child_type = Type::Unknown;
            }

            children.push_back(from_parent(self_fd, ent->d_name, child_type));
        }
        if (errno) {
            CWrapper::handle_error(-1, "readdir", "DIR*");
        }
        closedir(dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }

}
