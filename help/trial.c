#define WL 10
#define WC 100

//trial program

#include <menu.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>

void display(WINDOW *menu, int no,char **word) {
    for(int i=0;i<no;i++) {
        mvwprintw(menu,10+i,10,"%s",word[i]);
    }
}

void store_words(char **word) {
    FILE *items_file = fopen("data/opts","r");
    int i,no;
    fscanf(items_file,"%d",&no);

    char buff[10];

    for(i=0;i<no;i++) {
        fscanf(items_file,"%s",buff);
        strcpy(word[i],buff);
    }

    fclose(items_file);
}

int word_count() {
    FILE *items_file = fopen("data/opts", "r");
    int no;
    fscanf(items_file,"%d",&no);
    fclose(items_file);

    return no;
}

char** create_words(int no,int size) {
    int i;
    char** word = (char **)calloc(no,sizeof(char *));

    for(i=0;i<no;i++)
        word[i] = calloc(1,sizeof(char)*size);

    return word;
}

ITEM** assign_items(int no) {
    int i;
    ITEM** items = (ITEM **)calloc(no,sizeof(ITEM *));

    FILE *file = fopen("data/opts","r");
    for(i=0;i<no;i++) {
        items[i] = new_item("post","");
    }
    fclose(file);
    items[no] = NULL;

    return items;
}

void destroy_items(int no,ITEM **item) {
    int i;
    for(i=0;i<no;i++)
        free_item(item[i]);
}

void redraw_menu(MENU *menu, ITEM **items) {

    /*
    ITEM **new = (ITEM **)calloc(4,sizeof(ITEM*));

    new[0] = new_item("4","four");
    new[1] = new_item("5","five");
    new[2] = new_item("6","six");
    new[3] = new_item("7","seven");
    new[4] = NULL;
    */
    ITEM **new = assign_items(3);

    set_menu_items(menu,new);
    destroy_items(3,items);
}


int main() {

    ITEM **items,**new;
    ITEM *temp;
    MENU *menu;
    WINDOW *menu_win, *menu_sub_win;

    int nlines,ncols,starty,startx,c='a';
    int no,i,prev;

    FILE *tty = fopen("/dev/tty", "r+");
    SCREEN *screen = newterm(NULL, tty, tty);
    set_term(screen);
    start_color();
    cbreak();
    noecho();
    curs_set(0);

    getmaxyx(stdscr,nlines,ncols);

    menu_win = newwin(nlines-1,ncols-1,1,1);
    menu_sub_win = derwin(menu_win, nlines-2,ncols-2,1,1);
    
    wrefresh(menu_win);

        items = (ITEM **)calloc(3,sizeof(ITEM *));
        new = (ITEM **)calloc(3,sizeof(ITEM *));

        items[0] = new_item("one","");
        items[1] = new_item("two","");
        items[2] = new_item("three","");
        items[3] = NULL;

        menu = new_menu((ITEM **)items);

        set_menu_win(menu, menu_win);
        set_menu_sub(menu, menu_sub_win);

        box(menu_win,0,0);

        post_menu(menu);
        wrefresh(menu_win);

        getchar();

        /*
        unpost_menu(menu);
        new[0] = new_item("4","four");
        new[1] = new_item("5","five");
        new[2] = new_item("6","six");
        new[3] = NULL;
        */
        unpost_menu(menu);
        redraw_menu(menu, items);

        post_menu(menu);
        box(menu_win,0,0);
        wrefresh(menu_win);

        getchar();



    unpost_menu(menu);
    destroy_items(no,items);
    endwin();
    delscreen(screen);
    fclose(tty);

    return 0;
}
