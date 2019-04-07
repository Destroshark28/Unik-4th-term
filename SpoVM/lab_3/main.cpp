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
    const int KEY_ID_SEMAPHORE = 40;
    const int KEY_ID_SH_MEMORY = 41;
    const int SEMAPHORE_AMOUNT = 3;
    const int SERVER_SEMAPHORE_INDEX = 0;
    const int CLIENT_SEMAPHORE_INDEX = 1;
    const int KILL_SEMAPHORE_INDEX = 2;
    const char INITIAL_PATH[] = "/home/ilyshka/Desktop/Unik/SpoVM/lab_3/main.cpp";
}

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

using namespace std;

int main() {
    pid_t pid;
    int clientStatus;
    int semaphoreId, shMemoryId;
    key_t semaphoreKey, shMemoryKey;
    void *shMemoryAddress;
    struct sembuf semaphoreSet{};
    struct shmid_ds shMemoryStruct{};

    semaphoreKey = ftok(constants::INITIAL_PATH, constants::KEY_ID_SEMAPHORE);
    if (semaphoreKey == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    shMemoryKey = ftok(constants::INITIAL_PATH, constants::KEY_ID_SH_MEMORY);
    if (shMemoryKey == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    semaphoreId = createSemaphoreSet(semaphoreKey);
    if (semaphoreId == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    shMemoryId = shmget(shMemoryKey, 1, IPC_CREAT | SHM_R | SHM_W);
    if (shMemoryId == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        deleteSemaphoreSet(semaphoreId);
        exit(EXIT_FAILURE);
    }

    shMemoryAddress = shmat(shMemoryId, nullptr, 0);
    if (shMemoryAddress == nullptr) {
        cerr << "Error: " << strerror(errno) << endl;
        deleteSemaphoreSet(semaphoreId);
        shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);
        exit(EXIT_FAILURE);
    }

    pid = fork();
    switch (pid) {
        case -1: {
            cerr << "Error: " << strerror(errno) << endl;

            deleteSemaphoreSet(semaphoreId);
            shmdt(shMemoryAddress);
            shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);

            exit(EXIT_FAILURE);
        }
        case 0: {
            while (true) {
                semaphoreSet.sem_num = constants::SERVER_SEMAPHORE_INDEX;
                semaphoreSet.sem_op = -1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                if (semctl(semaphoreId, constants::KILL_SEMAPHORE_INDEX, GETVAL) == 1) break;

                cout << "Client got: " << (char *) shMemoryAddress << endl;

                semaphoreSet.sem_num = constants::CLIENT_SEMAPHORE_INDEX;
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

                strcpy((char *) shMemoryAddress, stringBuffer.c_str());
                stringBuffer.clear();

                semaphoreSet.sem_num = constants::SERVER_SEMAPHORE_INDEX;
                semaphoreSet.sem_op = 1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                semaphoreSet.sem_num = constants::CLIENT_SEMAPHORE_INDEX;
                semaphoreSet.sem_op = -1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                cout << "Press '0' to exit the program, or press any other key to continue..." << endl;

                if (cin.get() == '0') {
                    semaphoreSet.sem_num = constants::KILL_SEMAPHORE_INDEX;
                    semaphoreSet.sem_op = 1;
                    semaphoreSet.sem_flg = SEM_UNDO;
                    semop(semaphoreId, &semaphoreSet, 1);

                    semaphoreSet.sem_num = constants::SERVER_SEMAPHORE_INDEX;
                    semaphoreSet.sem_op = 1;
                    semaphoreSet.sem_flg = SEM_UNDO;
                    semop(semaphoreId, &semaphoreSet, 1);

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
    shmdt(shMemoryAddress);
    shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);

    return 0;
}
