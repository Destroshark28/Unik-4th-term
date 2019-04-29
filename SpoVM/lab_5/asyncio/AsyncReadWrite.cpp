#include <list>
#include <string>
#include <iostream>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <limits>

using namespace std;

namespace constants {
    const int KEY_ID_SEMAPHORE = 234;
    const int KEY_ID_SH_MEMORY = 2345;
    const int SEMAPHORE_AMOUNT = 1;
    const int SEMAPHORE_INDEX = 0;
    const char INITIAL_PATH[] = "/home/ilyshka/Desktop/Unik/SpoVM/lab_5/main.cpp";
}

typedef struct argReaderThread {
    list<string> *sourceFiles;
    int shMemoryId;
    int semaphoreId;
} readerThreadArgs;

typedef struct argWriterThread {
    string outputFileName;
    int shMemoryId;
    int semaphoreId;
} writerThreadArgs;

int createSemaphoreSet(key_t key) {
    int check = 0;
    int id = semget(key, constants::SEMAPHORE_AMOUNT, IPC_CREAT | SHM_R | SHM_W);
    if (id != -1) {
        int setArray[constants::SEMAPHORE_AMOUNT] = {0};
        check = semctl(id, 0, SETALL, setArray);
    }
    return (check == -1) ? check : id;
}

void deleteSemaphoreSet(int semaphoreId) {
    semctl(semaphoreId, 0, IPC_RMID, NULL);
}

void *getShMemory(key_t shMemoryId) {
    void *shMemoryAddress = shmat(shMemoryId, nullptr, 0);
    if (shMemoryAddress == nullptr) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    return shMemoryAddress;
}

key_t getSemaphoreKey() {
    return ftok(constants::INITIAL_PATH, constants::KEY_ID_SEMAPHORE);
}

key_t getShMemoryKey() {
    return ftok(constants::INITIAL_PATH, constants::KEY_ID_SH_MEMORY);
}

int getSemaphoreId() {
    key_t key = getSemaphoreKey();
    if (key == -1) {
        return -1;
    }
    return createSemaphoreSet(key);
}

int getShMemoryId() {
    key_t key = getShMemoryKey();
    if (key == -1) {
        return -1;
    }
    return shmget(key, 1, IPC_CREAT | SHM_R | SHM_W);
}

string readFromFile(const string &fileName) {
    return "AAA";
}

void writeToFile(const string &fileName, const string &data) {
    cout << data << endl;
}

void *readerThreadRoutine(void *arg) {
    struct sembuf semaphoreSet{};
    auto args = static_cast<readerThreadArgs *> (arg);

    for (auto &fileName : *args->sourceFiles) {

        strcpy((char *) getShMemory(args->shMemoryId), readFromFile(fileName).c_str());

        semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
        semaphoreSet.sem_op = 1;
        semaphoreSet.sem_flg = SEM_UNDO;
        semop(args->semaphoreId, &semaphoreSet, 1);

        semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
        semaphoreSet.sem_op = -1;
        semaphoreSet.sem_flg = SEM_UNDO;
        semop(args->semaphoreId, &semaphoreSet, 1);
    }

    semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
    semaphoreSet.sem_op = 2;
    semaphoreSet.sem_flg = SEM_UNDO;
    semop(args->semaphoreId, &semaphoreSet, 1);

    semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
    semaphoreSet.sem_op = -3;
    semaphoreSet.sem_flg = SEM_UNDO;
    semop(args->semaphoreId, &semaphoreSet, 1);

    return nullptr;
}

void *writerThreadRoutine(void *arg) {
    struct sembuf semaphoreSet{};
    auto args = static_cast<writerThreadArgs *> (arg);

    while (true) {
        semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
        semaphoreSet.sem_op = -1;
        semaphoreSet.sem_flg = SEM_UNDO;
        semop(args->semaphoreId, &semaphoreSet, 1);

        if (semctl(args->semaphoreId, constants::SEMAPHORE_INDEX, GETVAL) == 1) {
            semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
            semaphoreSet.sem_op = 3;
            semaphoreSet.sem_flg = SEM_UNDO;
            semop(args->semaphoreId, &semaphoreSet, 1);

            return nullptr;
        }

        string data = (char *) getShMemory(args->shMemoryId);
        writeToFile(args->outputFileName, data);

        semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
        semaphoreSet.sem_op = 1;
        semaphoreSet.sem_flg = SEM_UNDO;
        semop(args->semaphoreId, &semaphoreSet, 1);
    }
}

int startReaderThread(pthread_t &thread, list<string> &sourceFiles, int &shMemoryId, int &semaphoreId) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    auto *args = (readerThreadArgs *) malloc(sizeof(readerThreadArgs));
    args->sourceFiles = &sourceFiles;
    args->shMemoryId = shMemoryId;
    args->semaphoreId = semaphoreId;

    return pthread_create(&thread, &attr, readerThreadRoutine, args);
}

int startWriterThread(pthread_t &thread, string &outputFileName, int &shMemoryId, int &semaphoreId) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    auto *args = (writerThreadArgs *) malloc(sizeof(writerThreadArgs));
    args->outputFileName = outputFileName;
    args->shMemoryId = shMemoryId;
    args->semaphoreId = semaphoreId;

    return pthread_create(&thread, &attr, writerThreadRoutine, args);
}

extern "C" void concatFiles(list<string> &sourceFiles, string &outputFileName) {
    struct shmid_ds shMemoryStruct{};

    int semaphoreId = getSemaphoreId();
    int shMemoryId = getShMemoryId();
    pthread_t readerThread, writerThread;
    void *threadExitStatus;

    if (semaphoreId == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    if (shMemoryId == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    if (startWriterThread(writerThread, outputFileName, shMemoryId, semaphoreId)) {
        std::cout << "Error while creation thread" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (startReaderThread(readerThread, sourceFiles, shMemoryId, semaphoreId)) {
        std::cout << "Error while creation thread" << std::endl;
        exit(EXIT_FAILURE);
    }

    pthread_join(readerThread, &threadExitStatus);

    deleteSemaphoreSet(semaphoreId);
    shmdt(getShMemory(shMemoryId));
    shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);
}
