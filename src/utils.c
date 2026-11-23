#include "../include/utils.h"


void ncurses_init(void) {
    initscr();
    cbreak();
    noecho();
    start_color();
    keypad(stdscr,TRUE);
}

int handle_normal_mode(WINDOW *footer_window) {
    char input[256];
    memset(input,0,256);
    mvwhline(footer_window,1,1,' ',Editor.cursor_col-4);
    int index = 0;
    int c;
    int first_char = getch();
    if (first_char == ':') {
        mvwprintw(footer_window,1,1,":");
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
    } else if (first_char == 'i') {
        return 1;
    } else {
        mvwprintw(footer_window,1,index+1,"Invalid Key. Switching back to normal mode :)");
        wrefresh(footer_window);
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
        return 0;
    }
}

int handle_carriage_return(WINDOW *lines,WINDOW *editor) {
    // newline (when carriage return is pressed)
    Editor.cursor_row++;
    Editor.cursor_col = 5;
    mvwprintw(lines, Editor.cursor_row, 1, "%d",Editor.cursor_row);
    mvwhline(editor,Editor.cursor_row,6,' ',EDITOR_COLS-7);
    if (handle_line_buff() == -1) return -1;
    refresh();
    wrefresh(lines);
    wrefresh(editor);
    return 0;
}

int handle_line_buff(void) {
    // printf("%s\n",Editor.line_buff);
    // printf("Address: %p\n", (void *)Editor.line_buff);
    // printf("First byte: %d\n", (unsigned char)Editor.line_buff[0]);
    // free(Editor.line_buff);
    // memset(Editor.line_buff,0,0);
    //
    // Editor.line_buff = (char*)malloc(255);
    // Editor.line_buff_size = 255;
    // //Allocating 255bytes for a line... Later we could realloc() this if the size is exceeded
    free(Editor.line_buff);
    Editor.line_buff = NULL;

    Editor.line_buff = malloc(255);

    if (Editor.line_buff == NULL) {
        perror("malloc");
        return -1;
    }

    memset(Editor.line_buff, 0, 255);
    Editor.line_buff_size = 255;
    return 0;
}

// Code for the insert mode
// Calls the 'handle_normal_mode' function inside it.
// Returns -1 when the program is intended to be quitted like when :q,:wq etc.. is pressed
int handle_insert_mode(WINDOW *footer,WINDOW *lines,WINDOW *editor) {
    int line_idx=0; //Using this variable for indexing in Editor.line_buff
    if (handle_line_buff() == -1) return -1;
    while ((Editor.character = getch())) {
        // wmove(editor,Editor.cursor_row,Editor.cursor_col);
        // wrefresh(editor);
        if (Editor.character == '\033') { // Normal Mode end
            Editor.mode = handle_normal_mode(footer);
            if (Editor.mode == -1) {
                return -1;
            }
            wrefresh(editor);
            continue;
        }
        else if (Editor.character == '\n') {
            FILE *log = fopen("debug.log", "w");
            fprintf(log, "%s\n", Editor.line_buff);
            fclose(log);
            if (handle_carriage_return(lines,editor) == -1) return -1;
            line_idx = 0; // resets the line_idx,the indexing of Editor.line_buff would start from 0 again.
            continue;
        } else if (Editor.character == KEY_BACKSPACE || Editor.character == '\b' || Editor.character == 127) {
            // Backspace: move back and delete Editor.character
            if (Editor.cursor_col > 5) {
                Editor.cursor_col--;
                // replace the character to be deleted with " "(blank space)
                mvwprintw(editor,Editor.cursor_row,Editor.cursor_col," ");
                // Text cursor would be moved one cell back and text editing would be continued from there
                wmove(editor,Editor.cursor_row,Editor.cursor_col);
                line_idx--;
                Editor.line_buff[line_idx] = '\0';
            }
        } else if (Editor.character == KEY_LEFT) {
            // TODO: Fix Editor.character inserting (currently it overrides the character in the index++)
            if (Editor.cursor_col > 5) {
                Editor.cursor_col--;
                wmove(editor,Editor.cursor_row,Editor.cursor_col);
                line_idx--;
                refresh();
                wrefresh(editor);
            } else{}
            // It will run until we reach the end of the line
            // We could only get '\0' only if we go to end of the line using KEY_RIGHT
            while (Editor.line_buff[line_idx] != '\0') {
                int left_char = getch();
                if (left_char == '\033') break; // Bug possibilities currently
                // Handling KEY_LEFT again inside the line_buff editing while loop,to provide flexibility over using left arrow key
                else if (left_char == KEY_LEFT) {
                    if (Editor.cursor_col > 5) {
                        Editor.cursor_col--;
                        wmove(editor,Editor.cursor_row,Editor.cursor_col);
                        line_idx--;
                        refresh();
                        wrefresh(editor);
                    } else{}
                    break;
                }
                char edited_line[Editor.line_buff_size+1];
                int i = 0;
                // Making 'edited_line' and then duplicating it in the 'Editor.line_buff'
                // Re-rendering could be done here to refresh the editor window
                while (i != line_idx+1) {
                    edited_line[i] = Editor.line_buff[i];
                    i++;
                }
                edited_line[i] = left_char;
                // Appending characters of 'Editor.line_buff' after the index 'i'
                int j = i;
                i++;
                while(Editor.line_buff[j]) {
                    edited_line[i] = Editor.line_buff[j];
                    i++;
                    j++;
                }
                edited_line[i] = '\0';
                mvwhline(editor,Editor.cursor_row,5,' ',1);
                mvwprintw(editor, Editor.cursor_row,5,"%s",edited_line);
                line_idx++;
                Editor.cursor_col++;
                wmove(editor,Editor.cursor_row,line_idx);
                strcpy(Editor.line_buff,edited_line);
                refresh();
                wrefresh(editor);
                wrefresh(lines);
            }
            continue;
        }
        else {
            // Normal Editor.character: check right boundary before printing
            int max_x = getmaxx(editor);
            if (Editor.cursor_col < max_x - 1) {
                mvwprintw(editor, Editor.cursor_row, Editor.cursor_col, "%c", Editor.character);
                Editor.cursor_col++;

            }
        }
        // Updating Editor.line_buff buffer in the 'struct Editor Editor' struct
        if (line_idx < Editor.line_buff_size-1) {
            Editor.line_buff[line_idx] = Editor.character;
            line_idx++;
            Editor.line_buff[line_idx] = '\0';
        } else {
            // Allocating 64bytes more if the line_idx exceeded 255
            if ((Editor.line_buff = (char*)realloc(Editor.line_buff,Editor.line_buff_size+64)) == NULL) {
                // IF REALLOC FAILED
                perror("realloc");
                return -1;
            }
            Editor.line_buff_size += 64;
            Editor.line_buff[line_idx] = Editor.character;
            line_idx++;
            Editor.line_buff[line_idx] = '\0';
        }

        // refreshing the windows
        refresh();
        wrefresh(lines);
        wrefresh(editor);
    }
    return 0;
}
