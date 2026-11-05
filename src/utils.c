#include "../include/utils.h"

void ncurses_init(void) {
    initscr();
    cbreak();
    noecho();
    start_color();
}
