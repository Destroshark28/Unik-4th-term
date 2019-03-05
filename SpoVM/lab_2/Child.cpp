#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstring>


void print(int sig, siginfo_t *siginfo, void *context) {
    std::cout << "I'm CHILDDDD! My pid is " << std::to_string(getpid()) << std::endl;
    kill(getppid(), SIGUSR2);
}

int main() {
    struct sigaction act;
    memset(&act, '\0', sizeof(act));

    act.sa_sigaction = &print;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, nullptr);

    while (true);
}
