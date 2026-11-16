#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <curses.h>

void ncurses_init(void);
int handle_normal_mode(WINDOW *footer_window,int editor_cols);
