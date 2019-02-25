#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char **argv) {
    int array[10];

    initscr();

    pid_t pid = fork();
    switch (pid) {
        case -1: {
            printw("Error while creating child process\n");
            refresh();
            getch();
            exit(EXIT_FAILURE);
        }
        case 0: {
            WINDOW *subWindow = newwin(10, 30, 5, 20);
            box(subWindow, 0, 0);
            mvwprintw(subWindow, 1, 1, "Child:");
            wrefresh(subWindow);
            for (int i = 0; i < 10; ++i) {
                array[i] = i;
            }
            for (int i = 0; i < 10; ++i) {
                mvwprintw(subWindow, 2, i + 1, "%d", array[i]);
                wrefresh(subWindow);
            }
            sleep(2);
            delwin(subWindow);
            break;
        }
        default: {
            int childStatus;
            waitpid(pid, &childStatus, 0);
            if (WIFEXITED(childStatus)) {
                printw("Exited, status = %d\n", WEXITSTATUS(childStatus));
            } else if (WIFSIGNALED(childStatus)) {
                printw("Killed by signal %d\n", WTERMSIG(childStatus));
            } else if (WIFSTOPPED(childStatus)) {
                printw("Stopped by signal %d\n", WSTOPSIG(childStatus));
            }

            printw("Main: \n");
            for (int i = 0; i < 10; ++i) {
                printw("%d", array[i]);

            }
            refresh();
            getch();
            break;
        }
    }
    endwin();
    return 0;
}
