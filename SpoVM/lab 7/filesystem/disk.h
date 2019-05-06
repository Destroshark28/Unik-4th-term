#pragma once

#include <stdlib.h>

class Disk {
public:
    const static size_t BLOCK_SIZE = 4096;

    Disk();
    ~Disk();

    void open(const char *path);
    void create(const char *path, size_t totalBlocks);

    size_t size() const;

    void mount();
    void unMount();
    bool isMount() const;

    void read(size_t blockNumber,char *data);
    void write(size_t blockNumber,const char *data);

private:
    int fileDescriptor;
    size_t totalBlocksNumber;
    bool isMounted;

    void checkArgs(size_t blockNumber, const char *data);
};
