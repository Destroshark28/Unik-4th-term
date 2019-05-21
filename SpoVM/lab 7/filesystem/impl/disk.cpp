#include "../disk.h"

#include <stdexcept>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


Disk::Disk() : fileDescriptor(0), totalBlocksNumber(0), isMounted(false) {}

Disk::~Disk() {
    if (fileDescriptor > 0) {
        close(fileDescriptor);
        fileDescriptor = 0;
        unMount();
    }
}

void Disk::open(const char *path) {
    fileDescriptor = ::open(path, O_RDWR);
    if (fileDescriptor < 0) throw std::runtime_error(strerror(errno));
    struct stat statBuf{};
    ::stat(path, &statBuf);
    if (ftruncate(fileDescriptor, statBuf.st_size) < 0) throw std::runtime_error(strerror(errno));
    Disk::totalBlocksNumber = statBuf.st_size / BLOCK_SIZE;
}

void Disk::create(const char *path, size_t totalBlocks) {
    fileDescriptor = ::open(path, O_RDWR | O_CREAT);
    if (fileDescriptor < 0) throw std::runtime_error(strerror(errno));
    if (ftruncate(fileDescriptor, totalBlocks * BLOCK_SIZE) < 0) throw std::runtime_error(strerror(errno));
    Disk::totalBlocksNumber = totalBlocks;
}

size_t Disk::size() const {
    return totalBlocksNumber;
}

void Disk::mount() {
    isMounted = true;
}

void Disk::unMount() {
    isMounted = false;
}

bool Disk::isMount() const {
    return isMounted;
}

void Disk::read(size_t blockNumber, char *data) {
    checkArgs(blockNumber, data);
    if (lseek(fileDescriptor, blockNumber * BLOCK_SIZE, SEEK_SET) < 0) throw std::runtime_error(strerror(errno));
    if (::read(fileDescriptor, data, BLOCK_SIZE) != BLOCK_SIZE) throw std::runtime_error(strerror(errno));
}

void Disk::write(size_t blockNumber, const char *data) {
    checkArgs(blockNumber, data);
    if (lseek(fileDescriptor, blockNumber * BLOCK_SIZE, SEEK_SET) < 0) throw std::runtime_error(strerror(errno));
    if (::write(fileDescriptor, data, BLOCK_SIZE) != BLOCK_SIZE) throw std::runtime_error(strerror(errno));
}

void Disk::checkArgs(size_t blockNumber, const char *data) {
    if (blockNumber < 0) throw std::invalid_argument("blockNumber < 0");
    if (blockNumber >= totalBlocksNumber) throw std::invalid_argument("blockNumber > totalBlocksNumber");
    if (data == nullptr) throw std::invalid_argument("null pointer data");
}