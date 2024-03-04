#include "menu.h"
#include "string.h"

int display_menu()
{
    char **title = (char *[]){" [SNAKE] "};
    char **menu_items = (char *[]){"Start game", "Exit"};
    int padding = 29;
    int menu_items_count = 2;

    int x, y;
    getmaxyx(stdscr, y, x);

    int startx = (x - padding) / 2;
    int starty = (y - menu_items_count) / 2;

    WINDOW *menu_mwin = newwin(menu_items_count + 5, padding, starty, startx);
    box(menu_mwin, 0, 0);
    mvwprintw(menu_mwin, 0, (padding - strlen(*title)) / 2, title[0]);
    refresh();
    wrefresh(menu_mwin);

    int selected_item = 0;
    int ch = 0;

    while (ch != 10)    // Enter in ASCII
    {
        for (int i = 0; i < menu_items_count; i++)
        {
            if (i == selected_item)
            {
                wattron(menu_mwin, A_REVERSE);
            }
            mvwprintw(menu_mwin, i * 2 + 2, (padding - strlen(menu_items[i]))/2, menu_items[i]);
            wattroff(menu_mwin, A_REVERSE);
        }

        ch = getch();

        switch (ch)
        {
        case KEY_UP:
            if (selected_item > 0)
            {
                selected_item--;
            }
            break;
        case KEY_DOWN:
            if (selected_item < menu_items_count - 1)
            {
                selected_item++;
            }
            break;
        }
        wrefresh(menu_mwin);
    }
    delwin(menu_mwin);
    refresh();

    return selected_item;
}