#ifndef VIRTUALTREE_H
#define VIRTUALTREE_H

#include <deque>

#include "virtualnode.h"

class VirtualTree
{
public:
    VirtualTree();

    void load_dir(VirtualNode &node);

private:
    std::deque<VirtualNode> pool;
};

#endif // VIRTUALTREE_H
