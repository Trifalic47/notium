#include "../include/utils.h"
#include <curses.h>

int main(int argc, char const *argv[])
{
    ncurses_init();
    box(stdscr,0,0);

    /* TITLE */
    WINDOW *title = newwin(LINES/3.14,COLS-4,1,2);
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
    WINDOW *body = newwin(LINES/3,COLS-4,14,1);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);

    wattron(body,COLOR_PAIR(2));
    wattron(body,A_BOLD);
    mvwprintw(body,2,(COLS-4)/2,"[1] HOME");
    mvwprintw(body,3,(COLS-4)/2,"[2] NEW FILE");
    mvwprintw(body,4,(COLS-4)/2,"[3] OPEN FILE");
    mvwprintw(body,5,(COLS-4)/2,"[4] PREVIEW");
    mvwprintw(body,6,(COLS-4)/2,"[5] QUIT");
    wattroff(body,COLOR_PAIR(2));
    wattroff(body,A_BOLD);
    refresh();
    wrefresh(body);

    int ch;
    while ((ch=getch())) {
        if (ch-'5' == 0 || ch == 'q') {
            printw("QUITTING\n");
            break;
        } else if (ch - '2' == 0) {
            /* NEW FILE */
            clear();
            int col_number = 5;
            int line_number;
            line_number = 1;
            int input;
            int mode; // 0 for normal,1 for insert and 2 for visual (no mode is set as default)

            int editor_lines = (int)(LINES/1.07);
            int editor_cols = COLS-4;
            WINDOW *editor = newwin(editor_lines,editor_cols,1,1);
            scrollok(editor,TRUE);
            box(editor,0,0);

            /*Making lines with the line numbers
             * Using derwin() to make a new window for the line numbers
             */
            int lines_window_lines = editor_lines;
            WINDOW *lines = derwin(editor, editor_lines-2, 4, 0, 2);
            mvwprintw(lines,1,1,"1");

            init_pair(3,COLOR_BLACK,COLOR_CYAN);
            wattron(editor,COLOR_PAIR(3));
            mvwhline(editor,line_number,6,' ',editor_cols-7);
            refresh();
            wrefresh(lines);
            wrefresh(editor);

            while ((input=getch())) {
                if (input== 'i') { // Turn on the insert mode
                    mode = 1;
                    break;
                } else if (input == 'n') { // Turn on the normal mode
                    mode = 1;
                    break;
                } else if (input == 'v') { // Turn on the visual mode
                    mode = 2;
                    break;
                } else if (input == 'q') {
                    endwin();
                    exit(EXIT_SUCCESS);
                } else {
                    //Havent touched this one block yet
                }
            }

            int character;
            // Making insert mode
            if (mode == 1) {
                while ((character = getch())) {
                    if (character == '\033')
                        break;
                    if (character == '\n') {
                        line_number++;
                        col_number = 5;
                        mvwprintw(lines, line_number, 1, "%d", line_number);
                        mvwhline(editor,line_number,6,' ',editor_cols-7);
                        refresh();
                        wrefresh(lines);
                        wrefresh(editor);
                        continue;
                    }
                    mvwprintw(editor, line_number, col_number, "%c", character);
                    refresh();

                    wrefresh(lines);
                    wrefresh(editor);
                    col_number++;
                }
            }
            refresh();
            wrefresh(lines);
            wrefresh(editor);
        }
    }

    /* Closing */
    endwin();
    return EXIT_SUCCESS;
}
// wnoutrefresh(editor);
// doupdate();
