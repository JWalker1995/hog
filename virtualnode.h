#ifndef VIRTUALNODE_H
#define VIRTUALNODE_H

#include <cstddef>
#include <string>
#include <vector>

class VirtualNode
{
public:
    enum class Type {Unknown, Directory, File};

    int parent_fd;
    std::string self_name;
    int self_fd;

    std::size_t inode;
    Type type;

    std::vector<VirtualNode *> children;
};

#endif // VIRTUALNODE_H
