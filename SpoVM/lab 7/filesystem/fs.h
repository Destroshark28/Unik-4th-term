#pragma once

#include "disk.h"

#include <stdint.h>

class FileSystem {
public:
    const static uint32_t INODES_PER_BLOCK = 128;
    const static uint32_t POINTERS_PER_INODE = 5;
    const static uint32_t POINTERS_PER_BLOCK = 1024;

private:
    struct SuperBlock {
        uint32_t blocks;
        uint32_t iNodeBlocks;
        uint32_t iNodes;
    };

    struct INode {
        uint32_t valid;
        uint32_t size;
        uint32_t direct[POINTERS_PER_INODE];
        uint32_t indirect;
    };

    union Block {
        SuperBlock super;
        INode iNodes[INODES_PER_BLOCK];
        uint32_t pointers[POINTERS_PER_BLOCK];
        char data[Disk::BLOCK_SIZE];
    };

    uint32_t numberBlocks{};
    uint32_t iNodeBlocks{};
    uint32_t iNodes{};
    bool *freeBlocks{};

    Disk *disk = nullptr;

public:
    static bool format(Disk *disk);

    static void debug(Disk *disk);

    static void initializeINode(INode *node);

    bool mount(Disk *diskToMount);

    bool loadINode(size_t iNumber, INode *node);
    bool saveINode(size_t iNumber, INode *node);

    size_t allocFreeBlock();

    ssize_t create();
    bool remove(size_t iNumber);

    ssize_t stat(size_t iNumber);

    ssize_t read(size_t iNumber, char *data, size_t length, size_t offset);
    ssize_t write(size_t iNumber, char *data, size_t length, size_t offset);
};
