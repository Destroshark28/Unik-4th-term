

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>


int CreateProcess(char **pString) {
    pid_t pid;
    pid = fork();
    switch (pid) {
        case -1:
            perror("fork"); // произошла ошибка
            exit(1); //выход из родительского процесса
            break;
        case 0:
            printf(" CHILD: Это процесс-потомок!\n");
            execv(pString[1], pString);

        default:
            printf("PARENT: Это процесс-родитель!\n");
            printf("PARENT: Выход!\n");
            break;
    }
    return pid;
}


int main(int argc, char **argv) {

    {
        int val = 0, k;
        pid_t pid = -1;
        do {
            printf("What do u want to do?\n");
            scanf("%d", &val);

            switch (val) {

                case 1:
                    pid = CreateProcess(argv);
                    val = 0; //без этого уйдем в беск цикл
                    printf("PID потомка %d\n", pid);
                    break;

                case 2:
                    if (kill(pid, SIGTERM) == 0)
                        printf("done\n");
                    else
                        printf("error\n");
                    //написать обработчик, в его отсутствие работает как килл
                    // kill(pid, SIGKILL);
                    break;

                default:
                    break;
            }
        } while (val != 9);

        return 0;
    }
}