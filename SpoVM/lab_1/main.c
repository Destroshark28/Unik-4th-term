
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/mman.h>
#include <stdio.h>


int main(int argc, char **argv) {
    char *array = mmap(NULL, sizeof(char) * 10, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    //  initscr();

    pid_t pid = fork();
    switch (pid) {
        case -1: {
            exit(EXIT_FAILURE);
        }
        case 0: {
            for (int i = 0; i < 10; ++i) {
                array[i] = '0' + i;
            }
            execl("echo", "echo", array, NULL);
            sleep(2);
            break;
        }
        default: {
            int childStatus;
            waitpid(pid, &childStatus, 0);
            if (WIFEXITED(childStatus)) {
                printf("Exited, status = %d\n", WEXITSTATUS(childStatus));
            } else if (WIFSIGNALED(childStatus)) {
                printf("Killed by signal %d\n", WTERMSIG(childStatus));
            } else if (WIFSTOPPED(childStatus)) {
                printf("Stopped by signal %d\n", WSTOPSIG(childStatus));
            }

            printf("Main: \n");
            for (int i = 0; i < 10; ++i) {
                printf("%c", array[i]);

            }
            break;
        }
    }
    return 0;
}
