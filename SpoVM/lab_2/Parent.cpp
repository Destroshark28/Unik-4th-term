#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdio>
#include <list>
#include <pthread.h>

void *sendSignals(void *arg);

int startThread(std::pair<sigset_t &, std::list<pid_t> &> &args);

void initSigset(sigset_t &set);

int main(int argc, char **argv) {
    std::list<pid_t> pidList;
    sigset_t waitSet;

    initSigset(waitSet);

    std::pair<sigset_t &, std::list<pid_t> &> args(waitSet, pidList);
    if (startThread(args)) {
        std::cout << "Error while creation thread" << std::endl;
        exit(1);
    }

    std::cout << "Hello! This is the parent process." << std::endl;
    std::cout << "If you want to create new process, please press '+'" << std::endl;
    std::cout << "If you want to delete last process, please press '-'" << std::endl;
    std::cout << "If you want to quit, please press 'q'" << std::endl;

    char symbol;
    while (true) {
        std::cin.get(symbol);
        switch (symbol) {
            case '+':
                pid_t pid;
                pid = fork();
                switch (pid) {
                    case -1:
                        std::cout << "Error while creating child process!" << std::endl;
                        exit(EXIT_FAILURE);
                    case 0:
                        execv(argv[1], argv);
                        break;
                    default:
                        pidList.push_back(pid);
                        break;
                }
                break;

            case '-':
                if (pidList.empty()) {
                    std::cout << "There are no children to delete!" << std::endl;
                } else {
                    kill(pidList.back(), SIGKILL);
                    pidList.pop_back();
                }
                break;

            case 'q':
                if (!pidList.empty()) {
                    for (auto &childPid : pidList) {
                        kill(childPid, SIGKILL);
                    }
                    pidList.clear();
                }
                return 0;

            default:
                continue;
        }
    }
}

void initSigset(sigset_t &set) {
    sigemptyset(&set);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &set, nullptr);
}

int startThread(std::pair<sigset_t &, std::list<pid_t> &> &args) {
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    return pthread_create(&thread, &attr, sendSignals, &args);
}

void *sendSignals(void *arg) {
    auto params = static_cast<std::pair<sigset_t &, std::list<pid_t> &> *> (arg);
    auto &waitSet = params->first;
    auto &pidList = params->second;
    while (true) {
        for (auto &childPid : pidList) {
            kill(childPid, SIGUSR1);
            int sig;
            sigwait(&waitSet, &sig);
        }
        sleep(1);
    }
}