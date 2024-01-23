// STANDARD HEADER FILES //
#include <stdio.h>
#include <menu.h>
#include <curses.h>
#include <stdlib.h>
// STANDARD HEADER FILES //

// HEADER FILES //
#include "menu_display.h"
#include "print_title.h"
// HEADER FILES //

void menu_display(int argc, char **argv) {
    // Declaration //
    ITEM **my_items;
    MENU *my_menu;
    WINDOW *my_menu_win, *title_win;

    FILE *r = fopen("directories.txt","r");
    
    char *name=".";

    char store[argc][4];
    int c,i,cur_win_index;
    char search;
    int nlines,ncols,startx,starty;
    // Declaration //

    // Initialize curses //
    FILE *tty = fopen("/dev/tty", "r+");
    SCREEN *screen = newterm(NULL, tty, tty);
    set_term(screen);
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr,TRUE);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    // Initialize curses //

    // Initialize Variables //
    getmaxyx(stdscr, nlines, ncols);
    starty = 0;
    startx = 0;
    cur_win_index = 0;
    // Initialize Variables //


    // Create Menu Items and Menu //
    my_items = (ITEM **)calloc(argc, sizeof(ITEM *));
   
    for(i=0;i<argc;i++)  {
        sprintf(store[i],"%d",i+1);
        my_items[i] = new_item(store[i], argv[i]);
        set_item_userptr(my_items[i], name);
    }
    my_items[argc] = NULL;
    
    my_menu = new_menu((ITEM**)my_items);
    // Create Menu Items and Menu //

    
    // Create windows //

    title_win = newwin(3, ncols, starty, startx);
    my_menu_win = newwin(nlines-3, ncols, starty+3, startx);

    // Create windows //


    // sets menu main and sub windows ; sets menu mark//
    menu_opts_off(my_menu, O_NONCYCLIC);
    set_menu_spacing(my_menu, 0, 0, ncols/2);
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win,nlines-4,ncols-10,1,1));

    set_menu_format(my_menu, nlines-5, 2);
    set_menu_mark(my_menu, "*");
    // sets menu main and sub windows ; sets menu mark//
     
  
    // Print border around main window; add title//
    print_title(title_win, 1, startx, ncols, "File Manager", COLOR_PAIR(1));
   
    box(my_menu_win, 0, 0);
    // Print border around main window and title //
   
    
    // Render //
    wrefresh(title_win);

    post_menu(my_menu);
    wrefresh(my_menu_win);
    // Render //


    /* Main Loop */
    while((c=getch()) != 'q') {
        switch(c) {
            case 'j':
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;

            case 'k':
                menu_driver(my_menu, REQ_UP_ITEM);
                break;

            case 'l':
                menu_driver(my_menu, REQ_RIGHT_ITEM);
                break;

            case 'h':
                menu_driver(my_menu, REQ_LEFT_ITEM);
                break;

            case 10:
                {
                    FILE *f = fopen("selected.txt","w");
                    name = (char *)item_description(current_item(my_menu));
                    fprintf(f,"%s",name);
                    fclose(f);
                }
                break;
            
            case KEY_NPAGE:
                menu_driver(my_menu, REQ_SCR_DPAGE);
                break;

            case KEY_PPAGE:
                menu_driver(my_menu, REQ_SCR_UPAGE);
                break;
        }
        fscanf(r,"%d",stdout);
        for(i=0;i<argc;i++) {
            fscanf(r,"%s",argv[i]);
        }
        wrefresh(my_menu_win);
    }
    /* Main Loop */
  
    
    // Free memory //
    unpost_menu(my_menu);
    free_menu(my_menu);
    for(i=0;i<argc;i++)
        free_item(my_items[i]);

    // Free memory //

    endwin();
    fclose(tty);
}

