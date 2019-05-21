#include "../fs.h"

#include <algorithm>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>

void FileSystem::debug(Disk *disk) {
    Block block{};

    disk->read(0, block.data);

    printf("SuperBlock:\n");
    printf("\t%u blocks\n", block.super.blocks);
    printf("\t%u iNode blocks\n", block.super.iNodeBlocks);
    printf("\t%u iNodes\n", block.super.iNodes);

    Block iNodeBlock{};
    uint32_t numberINodes = sizeof(Block) / sizeof(INode);
    for (uint32_t i = 0; i < block.super.iNodeBlocks; i++) {
        disk->read(i + 1, iNodeBlock.data);
        for (uint32_t j = 0; j < numberINodes; j++) {
            if (iNodeBlock.iNodes[j].valid) {
                printf("INode %d:\n", j + i * numberINodes);
                printf("\tsize: %d bytes\n", iNodeBlock.iNodes[j].size);
                std::string directBlockString = "\tdirect blocks:";
                bool directFlag = false;
                for (unsigned int k : iNodeBlock.iNodes[j].direct) {
                    if (k) {
                        directFlag = true;
                        directBlockString += " ";
                        directBlockString += std::to_string(k);
                    }
                }
                if (directFlag) {
                    std::cout << directBlockString << std::endl;
                } else {
                    std::cout << "\tdirect blocks:\n";
                }
                if (iNodeBlock.iNodes[j].indirect) {
                    printf("\tindirect block: %d\n", iNodeBlock.iNodes[j].indirect);
                    Block indirectBlock{};
                    disk->read(iNodeBlock.iNodes[j].indirect, indirectBlock.data);
                    std::string indirectString = "\tindirect data blocks:";
                    bool indirectFlag = false;
                    for (unsigned int pointer : indirectBlock.pointers) {
                        if (pointer) {
                            indirectFlag = true;
                            indirectString += " ";
                            indirectString += std::to_string(pointer);
                        }
                    }
                    if (indirectFlag) {
                        std::cout << indirectString << std::endl;
                    }
                }

            }
        }
    }
}

bool FileSystem::format(Disk *disk) {
    if (disk->isMount()) {
        return false;
    }

    Block superBlock{};
    superBlock.super.blocks = disk->size();
    if (disk->size() % 10 == 0) {
        superBlock.super.iNodeBlocks = disk->size() / 10;
    } else {
        superBlock.super.iNodeBlocks = disk->size() / 10 + 1;
    }
    superBlock.super.iNodes = superBlock.super.iNodeBlocks * INODES_PER_BLOCK;
    int superBlockLocation = 0;
    disk->write(superBlockLocation, superBlock.data);

    Block emptyBlock = {0};
    for (uint32_t i = 1; i < superBlock.super.blocks; i++) {
        disk->write(i, emptyBlock.data);
    }

    return true;
}

bool FileSystem::mount(Disk *diskToMount) {

    if (this->disk) {
        return false;
    }

    Block superBlock{};
    diskToMount->read(0, superBlock.data);

    if (diskToMount->size() != superBlock.super.blocks) {
        return false;
    }

    if (diskToMount->size() % 10 == 0) {
        if (superBlock.super.iNodeBlocks != diskToMount->size() / 10) {
            return false;
        }
    } else {
        if (superBlock.super.iNodeBlocks != diskToMount->size() / 10 + 1) {
            return false;
        }
    }

    if (superBlock.super.iNodes != INODES_PER_BLOCK * superBlock.super.iNodeBlocks) {
        return false;
    }

    this->disk = diskToMount;
    diskToMount->mount();

    this->numberBlocks = superBlock.super.blocks;
    this->iNodeBlocks = superBlock.super.iNodeBlocks;
    this->iNodes = superBlock.super.iNodes;

    this->freeBlocks = new bool[this->numberBlocks];
    for (uint32_t i = 0; i < this->numberBlocks; i++) {
        this->freeBlocks[i] = true;
    }
    this->freeBlocks[0] = false;
    for (uint32_t i = 0; i < this->iNodeBlocks; i++) {
        this->freeBlocks[i + 1] = false;
    }

    Block iNodeBlock{};
    for (uint32_t i = 0; i < this->iNodeBlocks; i++) {
        diskToMount->read(i + 1, iNodeBlock.data);
        for (auto & iNode : iNodeBlock.iNodes) {
            if (iNode.valid) {
                for (unsigned int k : iNode.direct) {
                    if (k) {
                        this->freeBlocks[k] = false;
                    }
                }
                if (iNode.indirect) {
                    this->freeBlocks[iNode.indirect] = false;
                    Block indirectBlock{};
                    diskToMount->read(iNode.indirect, indirectBlock.data);
                    for (unsigned int pointer : indirectBlock.pointers) {
                        if (pointer) {
                            this->freeBlocks[pointer] = false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

void FileSystem::initializeINode(INode *node) {
    for (unsigned int & i : node->direct) {
        i = 0;
    }
    node->indirect = 0;
    node->size = 0;
}

ssize_t FileSystem::create() {
    ssize_t iNodeNumber = -1;
    for (uint32_t i = 0; i < this->iNodeBlocks; i++) {
        Block iNodeBlock{};
        disk->read(i + 1, iNodeBlock.data);
        for (uint32_t j = 0; j < INODES_PER_BLOCK; j++) {
            if (!iNodeBlock.iNodes[j].valid) {
                iNodeBlock.iNodes[j].valid = 1;
                initializeINode(&iNodeBlock.iNodes[j]);
                this->disk->write(i + 1, iNodeBlock.data);
                iNodeNumber = j + INODES_PER_BLOCK * i;
                break;
            }
        }
        if (iNodeNumber != -1) {
            break;
        }
    }
    return iNodeNumber;
}

bool FileSystem::remove(size_t iNumber) {
    INode nodeToRemove{};
    loadINode(iNumber, &nodeToRemove);

    if (!nodeToRemove.valid) {
        return false;
    }
    for (unsigned int & i : nodeToRemove.direct) {
        this->freeBlocks[i] = true;
        i = 0;
    }

    if (nodeToRemove.indirect) {
        Block indirectBlock{};
        this->disk->read(nodeToRemove.indirect, indirectBlock.data);
        for (unsigned int pointer : indirectBlock.pointers) {
            this->freeBlocks[pointer] = true;
        }
        nodeToRemove.indirect = 0;
    }
    nodeToRemove.valid = 0;
    nodeToRemove.size = 0;

    saveINode(iNumber, &nodeToRemove);

    return true;
}

ssize_t FileSystem::stat(size_t iNumber) {
    if (iNumber >= this->iNodes) {
        return -1;
    }

    INode statINode{};
    bool validINode = loadINode(iNumber, &statINode);
    if (!validINode) {
        return -1;
    }

    return statINode.size;
}

ssize_t FileSystem::read(size_t iNumber, char *data, size_t length, size_t offset) {

    if (iNumber >= this->iNodes) {
        return -1;
    }

    INode loadedINode{};
    bool validINode = loadINode(iNumber, &loadedINode);
    if (!validINode) {
        return -1;
    }

    if (loadedINode.size == offset) {
        return -1;
    }

    length = std::min(length, loadedINode.size - offset);

    uint32_t startBlock = offset / Disk::BLOCK_SIZE;
    uint32_t startByte = offset % Disk::BLOCK_SIZE;

    std::string dataString;
    Block readFromBlock{};
    size_t readIndex = length;
    uint32_t dataIndex = 0;
    while (startBlock < POINTERS_PER_INODE) {
        if (!loadedINode.direct[startBlock]) {
            startBlock++;
            startByte = 0;
            continue;
        }

        disk->read(loadedINode.direct[startBlock], readFromBlock.data);
        size_t blockSizeVar = Disk::BLOCK_SIZE;
        uint32_t dataSize = std::min(startByte + readIndex, blockSizeVar);
        uint32_t incrementer = startByte;

        while (incrementer < dataSize) {
            dataString += readFromBlock.data[incrementer];
            incrementer++;
            dataIndex++;
        }
        readIndex = readIndex - dataSize + startByte;
        startByte = 0;
        startBlock++;
    }

    Block indirectBlock{};
    if (readIndex && loadedINode.indirect) {
        disk->read(loadedINode.indirect, indirectBlock.data);
        startBlock = startBlock - POINTERS_PER_INODE;
        while (startBlock < POINTERS_PER_BLOCK) {
            if (!indirectBlock.pointers[startBlock]) {
                startBlock++;
                startByte = 0;
                continue;
            }
            disk->read(indirectBlock.pointers[startBlock], readFromBlock.data);
            size_t blockSizeVar = Disk::BLOCK_SIZE;
            uint32_t dataSize = std::min(startByte + readIndex, blockSizeVar);
            uint32_t incrementer = startByte;
            while (incrementer < dataSize) {
                dataString += readFromBlock.data[incrementer];
                dataIndex++;
                incrementer++;
            }
            readIndex = readIndex - dataSize + startByte;
            startByte = 0;
            startBlock++;

        }
    }

    memcpy(data, dataString.c_str(), dataString.size());
    return dataIndex;
}

size_t FileSystem::allocFreeBlock() {
    for (uint32_t i = 0; i < this->numberBlocks; i++) {
        if (this->freeBlocks[i]) {
            this->freeBlocks[i] = false;
            return i;
        }
    }
    return 0;
}

ssize_t FileSystem::write(size_t iNumber, char *data, size_t length, size_t offset) {
    if (iNumber >= this->iNodes) {
        printf("First check failed\n");
        return -1;
    }

    INode loadedINode{};
    bool validINode = loadINode(iNumber, &loadedINode);
    if (!validINode) {
        return -1;
    }
    size_t readIndex = length;
    size_t blockSize = Disk::BLOCK_SIZE;

    uint32_t startBlock = offset / Disk::BLOCK_SIZE;
    uint32_t startByte = offset % Disk::BLOCK_SIZE;

    std::string dataString = data;
    Block readFromBlock{};
    uint32_t dataIndex = 0;
    while (readIndex > 0 && startBlock < POINTERS_PER_INODE) {
        if (!loadedINode.direct[startBlock]) {
            loadedINode.direct[startBlock] = allocFreeBlock();
            if (!loadedINode.direct[startBlock]) {
                startBlock++;
                continue;
            }
        }

        disk->read(loadedINode.direct[startBlock], readFromBlock.data);
        uint32_t dataSize = std::min(startByte + readIndex, blockSize);
        uint32_t incrementer = startByte;

        while (incrementer < dataSize) {
            readFromBlock.data[incrementer] = dataString[dataIndex];
            incrementer++;
            dataIndex++;
        }
        loadedINode.size += dataSize - startByte;
        saveINode(iNumber, &loadedINode);
        this->disk->write(loadedINode.direct[startBlock], readFromBlock.data);
        readIndex = readIndex - dataSize + startByte;
        startByte = 0;
        startBlock++;
    }

    Block indirectBlock = {0};
    if (readIndex || startBlock > POINTERS_PER_INODE) {
        if (!loadedINode.indirect) {
            loadedINode.indirect = allocFreeBlock();
            if (!loadedINode.indirect) {
                loadedINode.size += dataIndex;
                return dataIndex;
            }
        } else {
            disk->read(loadedINode.indirect, indirectBlock.data);
        }
        startBlock = startBlock - POINTERS_PER_INODE;
        while (startBlock < POINTERS_PER_BLOCK && readIndex > 0) {
            if (!indirectBlock.pointers[startBlock]) {
                indirectBlock.pointers[startBlock] = allocFreeBlock();
                if (!indirectBlock.pointers[startBlock]) {
                    startBlock++;
                    startByte = 0;
                    continue;
                }
            }
            disk->read(indirectBlock.pointers[startBlock], readFromBlock.data);
            blockSize = Disk::BLOCK_SIZE;
            uint32_t dataSize = std::min(startByte + readIndex, blockSize);
            uint32_t incrementer = startByte;
            while (incrementer < dataSize) {
                readFromBlock.data[incrementer] = dataString[dataIndex];
                dataIndex++;
                incrementer++;
            }
            loadedINode.size += dataSize - startByte;
            this->disk->write(indirectBlock.pointers[startBlock], readFromBlock.data);
            readIndex = readIndex - dataSize + startByte;
            startByte = 0;
            startBlock++;

        }
        this->disk->write(loadedINode.indirect, indirectBlock.data);
    }

    validINode = saveINode(iNumber, &loadedINode);
    if (!validINode) {
        return -1;
    }

    return dataIndex;
}

bool FileSystem::loadINode(size_t iNumber, INode *node) {
    Block nodeBlock{};
    this->disk->read(iNumber / INODES_PER_BLOCK + 1, nodeBlock.data);
    *node = nodeBlock.iNodes[iNumber % INODES_PER_BLOCK];
    return node->valid != 0;
}

bool FileSystem::saveINode(size_t iNumber, INode *node) {
    Block nodeBlock{};
    this->disk->read(iNumber / INODES_PER_BLOCK + 1, nodeBlock.data);
    nodeBlock.iNodes[iNumber % INODES_PER_BLOCK] = *node;
    this->disk->write(iNumber / INODES_PER_BLOCK + 1, nodeBlock.data);
    return node->valid != 0;
}
