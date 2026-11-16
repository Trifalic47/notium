#include "../include/utils.h"

void ncurses_init(void) {
    initscr();
    cbreak();
    noecho();
    start_color();
    keypad(stdscr,TRUE);
}

int handle_normal_mode(WINDOW *footer_window,int editor_cols) {
    char input[256];
    memset(input,0,256);
    mvwhline(footer_window,1,1,' ',editor_cols-4);
    int index = 0;
    int c;
    int first_char;
    if ((first_char=getch()) == ':') {
        mvwprintw(footer_window,1,index+1,":");
        index++;
        wrefresh(footer_window);
        while (1) {
            c = getch();
            if (c == '\n') {
                break;
            }
            input[index-1] = c;
            if (index == 255) {
                mvwprintw(footer_window,1,1,"Command limit exceeded\n");
                refresh();
                wrefresh(footer_window);
                break;
            }
            if (c == '\b' || c == 127 || c == KEY_BACKSPACE) {
                if (index <= 0) continue;
                mvwprintw(footer_window,1,index+1," ");
                wmove(footer_window,1,index+1);
                index--;
                refresh();
                wrefresh(footer_window);
                continue;
            }
            mvwprintw(footer_window,1,index+1,"%c",c);
            index++;
            refresh();
            wrefresh(footer_window);
        }
    } else if ((first_char=getch()) == 'i') {
        return 1;
    }
    if (strcmp(input,"wq") == 0) {
        /* TODO: Add saving functionality */
        endwin();
        return -1;
    } else if (strcmp(input,"q") == 0) {
        endwin();
        return -1;
    } else if (strcmp(input,"w") == 0) {
        // Have to implement file saving functionality
        return 0;
    }
    else {
        // mvwhline(footer_window,1,1,' ',COLS);
        // refresh();
        // wrefresh(footer_window);
        return 0;
    }
}
