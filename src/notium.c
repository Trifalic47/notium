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

            int editor_lines = (int)(LINES/1.1);
            int editor_cols = COLS-4;
            WINDOW *editor = newwin(editor_lines,editor_cols,1,1);
            scrollok(editor,TRUE);
            box(editor,0,0);

            int footer_lines = (LINES-editor_lines)-1;
            int footer_cols = COLS-4;
            WINDOW *footer = newwin(footer_lines,footer_cols,editor_lines+1,1);
            box(footer,0,0);

            /*Making lines with the line numbers
             * Using derwin() to make a new window for the line numbers
             */
            int lines_window_lines = editor_lines;
            WINDOW *lines = derwin(editor, editor_lines-2, 4, 0, 2);
            mvwprintw(lines,1,1,"1");

            init_pair(3,COLOR_WHITE,COLOR_BLACK);
            wattron(editor,COLOR_PAIR(3));
            mvwhline(editor,line_number,6,' ',editor_cols-7);
            refresh();
            wrefresh(lines);
            wrefresh(editor);
            wrefresh(footer);

            while ((input=getch())) {
                if (input== 'i') { // Turn on the insert mode
                    mode = 1;
                    break;
                } else if (input == 'n') { // Turn on the normal mode
                    mode = 0;
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
                    if (character == '\033') {
                        mode = 0; // switch to normal mode
                        char input[256];
                        memset(input,0,256);
                        mvwhline(footer,1,1,' ',editor_cols-4);
                        int index = 0;
                        int c;
                        // wgetnstr(editor,input,sizeof(input)-1);
                        while (1) {
                            c = getch();
                            if (c == '\n') {
                                break;
                            }
                            input[index] = c;
                            if (index == 10) {
                                mvwprintw(footer,1,1,"Command limit exceeded\n");
                                refresh();
                                wrefresh(footer);
                                break;
                            }
                            index++;
                            mvwprintw(footer,1,index+1,"%c",c);
                            refresh();
                            wrefresh(footer);
                        }
                        if (strcmp(input,":wq") == 0) {
                            // ADD SAVING FUNCTONALITY
                            endwin();
                            exit(EXIT_SUCCESS);
                        } else if (strcmp(input,":q") == 0) {
                            endwin();
                            exit(EXIT_SUCCESS);
                        } else if (strcmp(input,":w") == 0) {
                            // Have to implement file saving functionality
                        } else {
                            // mvwhline(footer,1,1,' ',COLS);
                            // refresh();
                            // wrefresh(footer);
                        }
                    }
                    if (character == '\n') {
                        // newline (when enter key is pressed)
                        line_number++;
                        col_number = 5;
                        mvwprintw(lines, line_number, 1, "%d", line_number);
                        mvwhline(editor,line_number,6,' ',editor_cols-7);
                        refresh();
                        wrefresh(lines);
                        wrefresh(editor);
                        continue;
                    } else if (character == KEY_BACKSPACE || character == '\b' || character == 127) {
                        // Backspace: move back and delete character
                        if (col_number > 0) {
                            col_number--;
                            mvwdelch(editor, line_number, col_number);
                        }
                    } else {
                        // Normal character: check right boundary before printing
                        int max_x = getmaxx(editor);
                        if (col_number < max_x - 1) {
                            mvwprintw(editor, line_number, col_number, "%c", character);
                            col_number++;
                        }
                    }

                    // Consolidated refresh at the end of the loop
                    refresh();
                    wrefresh(lines);
                    wrefresh(editor);
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
