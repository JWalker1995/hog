#ifndef VIRTUALNODEPOOL_H
#define VIRTUALNODEPOOL_H

#include <deque>

#include "virtualnode.h"

class VirtualNodePool {
public:
    typedef std::deque<VirtualNode>::size_type IndexType;

    class RangeException : public std::exception {
    public:
        RangeException(IndexType attempted, IndexType size)
            : msg("Tried to access VirtualNode at " + std::to_string(attempted) + ", outside range of [1 - " + std::to_string(size - 1) + "]")
        {}

        virtual const char *what() const noexcept {
            return msg.c_str();
        }

    private:
        std::string msg;
    };

    VirtualNodePool() {
        // Make sure all allocated indices are nonzero
        pool.emplace_back();
    }

    VirtualNode *allocate() {
        IndexType res = pool.size();
        pool.emplace_back();
        return res;
    }

    VirtualNode *get(IndexType index) {
        if (index < 1 || index >= pool.size()) {
            throw RangeException(index, pool.size());
        }

        return &pool[index];
    }

private:
    std::deque<VirtualNode> pool;
};

#endif // VIRTUALNODEPOOL_H
