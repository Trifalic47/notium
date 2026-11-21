#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <curses.h>

#ifndef UTILS_H
#define UTILS_H

#define EDITOR_ROWS (int)(LINES/1.1)
#define EDITOR_COLS (int)COLS-4

struct Editor {
    int cursor_row;
    int cursor_col;
    int mode;
    char *line_buff;
};

extern struct Editor Editor;

void ncurses_init(void);
int handle_normal_mode(WINDOW *footer_window);
void handle_carriage_return(WINDOW *lines,WINDOW *editor);

#endif
