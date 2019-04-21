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

namespace constants {
    const int KEY_ID_SEMAPHORE = 234;
    const int KEY_ID_SH_MEMORY = 2345;
    const int SEMAPHORE_AMOUNT = 1;
    const int SEMAPHORE_INDEX = 0;
    const char INITIAL_PATH[] = "/home/ilyshka/Desktop/Unik/SpoVM/lab_3/main.cpp";
}

using namespace std;

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

int main() {
    struct sembuf semaphoreSet{};
    struct shmid_ds shMemoryStruct{};

    int semaphoreId = getSemaphoreId();
    if (semaphoreId == -1) {
        cerr << "Error:  " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    int shMemoryId = getShMemoryId();
    if (shMemoryId == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    switch (pid) {
        case -1: {
            cerr << "Error: " << strerror(errno) << endl;
            deleteSemaphoreSet(semaphoreId);
            shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);
            exit(EXIT_FAILURE);
        }
        case 0: {
            while (true) {
                semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
                semaphoreSet.sem_op = -1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                if (semctl(semaphoreId, constants::SEMAPHORE_INDEX, GETVAL) == 1) break;

                cout << "Client got: " << (char *) getShMemory(shMemoryId) << endl;

                semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
                semaphoreSet.sem_op = 1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);
            }
            exit(EXIT_SUCCESS);
        }
        default: {
            string stringBuffer;

            while (true) {
                cout << "Server - enter the string" << endl;
                getline(cin, stringBuffer);
                strcpy((char *) getShMemory(shMemoryId), stringBuffer.c_str());
                stringBuffer.clear();

                semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
                semaphoreSet.sem_op = 1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
                semaphoreSet.sem_op = -1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                cout << "Press '0' to exit the program, or press any other key to continue..." << endl;

                if (cin.get() == '0') {
                    semaphoreSet.sem_num = constants::SEMAPHORE_INDEX;
                    semaphoreSet.sem_op = 2;
                    semaphoreSet.sem_flg = SEM_UNDO;
                    semop(semaphoreId, &semaphoreSet, 1);

                    int clientStatus;
                    waitpid(pid, &clientStatus, 0);
                    if (WIFEXITED(clientStatus)) {
                        cout << "Client has exited with status: " << WEXITSTATUS(clientStatus) << endl;
                    }
                    break;
                }

                cin.clear();
            }
        }
    }

    deleteSemaphoreSet(semaphoreId);
    shmdt(getShMemory(shMemoryId));
    shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);

    return 0;
}
