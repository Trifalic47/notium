#include "../include/utils.h"
#include <curses.h>

int main(int argc, char const *argv[])
{
    ncurses_init();
    box(stdscr,0,0);

    /* TITLE */
    WINDOW *title = newwin(12,COLS-4,1,2);
    char *title_str = "NOTIUM";

    box(title,0,0);

    init_pair(1,COLOR_RED,COLOR_BLACK);

    wattron(title,COLOR_PAIR(1));
    wattron(title,A_BOLD);
    wattron(title,A_BLINK);

    mvwprintw(title,3,(COLS/2)-20," _   _  ___ _____ ___ _   _ __  __");
    mvwprintw(title,4,(COLS/2)-20,"| \\ | |/ _ \\_   _|_ _| | | |  \\/  |");
    mvwprintw(title,5,(COLS/2)-20,"|  \\| | | | || |  | || | | | |\\/| |");
    mvwprintw(title,6,(COLS/2)-20,"| |\\  | |_| || |  | || |_| | |  | |");
    mvwprintw(title,7,(COLS/2)-20,"|_| \\_|\\___/ |_| |___|\\___/|_|  |_|");


    wattroff(title,COLOR_PAIR(1));
    wattroff(title,A_BOLD);
    wattroff(title,A_BLINK);

    refresh();
    wrefresh(title);

    /* BODY WINDOW */
    WINDOW *body = newwin(20,COLS-4,14,1);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);

    wattron(body,COLOR_PAIR(2));
    wattron(body,A_BOLD);
    mvwprintw(body,2,(COLS-4)/2,"[1] HOME");
    mvwprintw(body,3,(COLS-4)/2,"[2] NEW FILE");
    mvwprintw(body,4,(COLS-4)/2,"[3] OPEN FILE");
    mvwprintw(body,5,(COLS-4)/2,"[4] PREVIEW");
    mvwprintw(body,6,(COLS-4)/2,"[5] QUIT");
    refresh();
    wrefresh(body);

    /* Closing */
    getch();
    endwin();
    return EXIT_SUCCESS;
}
