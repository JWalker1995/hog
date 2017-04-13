#ifndef SOURCENODE_H
#define SOURCENODE_H

#include <deque>

class SourceNode
{
public:
    SourceNode(int self_fd)
        : self_fd(self_fd)
    {}

private:
    enum class Type {Unknown, Directory, File};

    SourceNode() {}
    static SourceNode from_parent(int parent_fd, const char *self_name, Type type) {
        SourceNode res;
        res.parent_fd = parent_fd;
        res.self_name = self_name;
        res.self_fd = 0;
        res.type = type;
        return res;
    }

    void load_dir();

    int parent_fd;
    std::string self_name;
    int self_fd;

    Type type;

    std::deque<SourceNode> children;
};

#endif // SOURCENODE_H
