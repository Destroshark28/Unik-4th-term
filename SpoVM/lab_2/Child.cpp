#include <iostream>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

namespace patch {
    template<typename T>
    std::string to_string(const T &n) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

using namespace std;

int main() {
    int receiver = 0;

    string message = "I'm CHILDDDD! My pid is ";
    string pidStr = patch::to_string((int) getpid());
    message += pidStr;

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGKILL);
    sigprocmask(SIG_BLOCK, &set, nullptr);

    while (true) {
        sigwait(&set, &receiver);
        cout << message.c_str() << endl;;
        kill(getppid(), SIGUSR2);
    }
}
