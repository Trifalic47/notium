#include "../include/utils.h"

int main(int argc, char const *argv[])
{
    ncurses_init();
    box(stdscr,0,0);


const char *str =
" _   _  ___ _____ ___ _   _ __  __\n"
"| \\ | |/ _ \\_   _|_ _| | | |  \\/  |\n"
"|  \\| | | | || |  | || | | | |\\/| |\n"
"| |\\  | |_| || |  | || |_| | |  | |\n"
"|_| \\_|\\___/ |_| |___|\\___/|_|  |_|\n";

    /* TITLE */
    WINDOW *title = newwin(15,COLS-4,1,2);
    char *title_str = "NOTIUM";

    box(title,0,0);

    init_pair(1,COLOR_RED,COLOR_BLACK);

    wattron(title,COLOR_PAIR(1));
    wattron(title,A_BOLD);
    wattron(title,A_BLINK);

    mvwprintw(title,3,(COLS/2)-20," _   _  ___ _____ ___ _   _ __  __");
    mvwprintw(title,4,(COLS/2)-20,"| \\ | |/ _ \\_   _|_ _| | | |  \\/  |\n");
    mvwprintw(title,5,(COLS/2)-20,"|  \\| | | | || |  | || | | | |\\/| |\n");
    mvwprintw(title,6,(COLS/2)-20,"| |\\  | |_| || |  | || |_| | |  | |\n");
    mvwprintw(title,7,(COLS/2)-20,"|_| \\_|\\___/ |_| |___|\\___/|_|  |_|\n");


    wattroff(title,COLOR_PAIR(1));
    wattroff(title,A_BOLD);
    wattroff(title,A_BLINK);

    refresh();
    wrefresh(title);

    /* Closing */
    getch();
    endwin();
    return EXIT_SUCCESS;
}
