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
    const int SH_MEMORY_SIZE = 10;
    const int SEMAPHORE_AMOUNT = 4;
    const int SERVER_SEMAPHORE_INDEX = 0;
    const int CLIENT_SEMAPHORE_INDEX = 1;
    const int KILL_SEMAPHORE_INDEX = 2;
    const int CLIENT_ERROR_SEMAPHORE_INDEX = 3;
    const short SET_ARRAY[SEMAPHORE_AMOUNT] = {0};
    const char INITIAL_PATH[] = "/dev/null";
}

int createSemaphoreSet(key_t key) {
    int id;
    int check = 0;

    id = semget(key, constants::SEMAPHORE_AMOUNT, IPC_CREAT | SHM_R | SHM_W);
    if (id != -1) {
        check = semctl(id, 0, SETALL, const_cast<short *>(constants::SET_ARRAY));
    }

    return (check == -1) ? check : id;
}

void deleteSemaphoreSet(int semaphoreId) {
    semctl(semaphoreId, 0, IPC_RMID, NULL);
}

void *mapSharedMemory(int shmId) {
    void *memoryAddress;
    memoryAddress = shmat(shmId, nullptr, 0);
    return memoryAddress;
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
    if (semaphoreKey == (key_t) -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    shMemoryKey = ftok(constants::INITIAL_PATH, constants::KEY_ID_SH_MEMORY);
    if (shMemoryKey == (key_t) -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    semaphoreId = createSemaphoreSet(semaphoreKey);
    if (semaphoreId == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    shMemoryId = shmget(shMemoryKey, constants::SH_MEMORY_SIZE, IPC_CREAT | SHM_R | SHM_W);
    if (shMemoryId == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        deleteSemaphoreSet(semaphoreId);
        exit(EXIT_FAILURE);
    }

    shMemoryAddress = (char *) mapSharedMemory(shMemoryId);
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

                if (semctl(semaphoreId, constants::KILL_SEMAPHORE_INDEX, GETVAL) == 1) {
                    shmdt(shMemoryAddress);
                    break;
                }

                shMemoryAddress = (char *) mapSharedMemory(shMemoryId);
                if (shMemoryAddress == nullptr) {
                    cerr << "Error: " << strerror(errno) << endl;

                    semaphoreSet.sem_num = constants::CLIENT_ERROR_SEMAPHORE_INDEX;
                    semaphoreSet.sem_op = 1;
                    semaphoreSet.sem_flg = SEM_UNDO;
                    semop(semaphoreId, &semaphoreSet, 1);

                    semaphoreSet.sem_num = constants::CLIENT_SEMAPHORE_INDEX;
                    semaphoreSet.sem_op = 1;
                    semaphoreSet.sem_flg = SEM_UNDO;
                    semop(semaphoreId, &semaphoreSet, 1);

                    exit(EXIT_FAILURE);
                }

                cout << "Client got: " << (char *) shMemoryAddress << endl;

                semaphoreSet.sem_num = constants::CLIENT_SEMAPHORE_INDEX;
                semaphoreSet.sem_op = 1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);
            }
            exit(EXIT_SUCCESS);
        }
        default: {
            int currentPosition = 0;
            bool isReadyForInput = true;
            string stringBuffer;
            stringBuffer.resize(constants::SH_MEMORY_SIZE, '\0');

            while (true) {
                memset(shMemoryAddress, '\0', 1);

                if (isReadyForInput) {
                    cout << "Server: Please, enter the string" << endl;
                    getline(cin, stringBuffer);
                    isReadyForInput = false;
                }

                string tempBuffer;
                int newLength = 0;
                tempBuffer.append(stringBuffer, 0, constants::SH_MEMORY_SIZE - 1);
                currentPosition = tempBuffer.length();
                strcpy((char *) shMemoryAddress, const_cast<char *>(tempBuffer.c_str()));

                tempBuffer.clear();
                newLength = stringBuffer.length() - currentPosition;
                if (newLength > 0) {
                    tempBuffer.append(stringBuffer, currentPosition, newLength);
                }
                stringBuffer = tempBuffer;

                semaphoreSet.sem_num = constants::SERVER_SEMAPHORE_INDEX;
                semaphoreSet.sem_op = 1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                semaphoreSet.sem_num = constants::CLIENT_SEMAPHORE_INDEX;
                semaphoreSet.sem_op = -1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                if (semctl(semaphoreId, constants::CLIENT_ERROR_SEMAPHORE_INDEX, GETVAL) > 0) {
                    break;
                }

                if (stringBuffer.empty()) {
                    isReadyForInput = true;
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
                            cout << "Client has exited with status value: " << WEXITSTATUS(clientStatus) << endl;
                        }
                        break;
                    }
                    stringBuffer.clear();

                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << endl;
                }
            }
        }
    }

    deleteSemaphoreSet(semaphoreId);
    shmdt(shMemoryAddress);
    shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);

    return 0;
}
