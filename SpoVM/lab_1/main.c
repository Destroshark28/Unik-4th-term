#include <curses.h>
#include <unistd.h>

int main(int argc, char **argv) {
    initscr();
    printw("Main");

    WINDOW *subwindow = newwin(10, 30, 5, 20);
    box(subwindow, 0, 0);
    refresh();
    mvwprintw(subwindow, 1, 1, "subwindow");
    wrefresh(subwindow);


    getch();
    getch();

    delwin(subwindow);
    endwin();
    return 0;
}
