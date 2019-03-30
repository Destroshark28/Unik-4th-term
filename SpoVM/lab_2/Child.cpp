#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <bits/sigaction.h>

void print(int sig, siginfo_t *siginfo, void *context);

void initSigaction();

int main() {
    initSigaction();
    while (true);
}

void initSigaction() {
    struct sigaction act{};
    act.sa_sigaction = print;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, nullptr);
}

void print(int sig, siginfo_t *siginfo, void *context) {
    std::string str = "I'm CHILD! My pid is " + std::to_string(getpid());
    for (auto &c:str) {
        std::cout << c;
    }
    std::cout << std::endl;
    kill(getppid(), SIGUSR2);
}

