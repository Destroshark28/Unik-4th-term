#include <iostream>
#include <cstring>
#include <unistd.h>

int main() {

    while (true) {
        sleep(5);
        std::cout << "Child!!!!!!!!1 " << std::endl;
    }
}
