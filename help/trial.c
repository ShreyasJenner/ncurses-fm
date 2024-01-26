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

ITEM** assign_items(int no, char **word) {
    int i;
    ITEM** items = (ITEM **)calloc(no,sizeof(ITEM *));

    FILE *file = fopen("data/opts","r");
    size_t cap[no];
    for(i=0;i<no;i++) {
        getline(&word[i], &cap[i], file);
        if(word[i][strlen(word[i])-1] = '\n')
            word[i][strlen(word[i])-1] = '\0';
    }
    for(i=0;i<no;i++) {
        items[i] = new_item(word[i],"");
    }
    items[no] = NULL;
    fclose(file);

    return items;
}

void destroy_items(int no,ITEM **item) {
    int i;
    for(i=0;i<no;i++)
        free_item(item[i]);
}

ITEM** redraw_menu(MENU *menu, ITEM **items, char** word) {

    ITEM **new = assign_items(3,word);

    set_menu_items(menu,new);

    return new;
}


int main() {

    ITEM **items,**new,**temp;
    MENU *menu;
    WINDOW *menu_win, *menu_sub_win;

    int nlines,ncols,starty,startx,c='a';
    int no,i,prev;
    char **word,**tempword;

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
        word = (char **)calloc(WC,sizeof(char)*10);

        /*
        items[0] = new_item("one","");
        items[1] = new_item("two","");
        items[2] = new_item("three","");
        items[3] = NULL;
        */

        menu = new_menu((ITEM **)items);

        set_menu_win(menu, menu_win);
        set_menu_sub(menu, menu_sub_win);


        temp = items;
        items = redraw_menu(menu, items, word);
        destroy_items(3,temp);

        post_menu(menu);
        box(menu_win,0,0);
        wrefresh(menu_win);
        size_t cap[no];

        while((c=getchar())!='q') {

            unpost_menu(menu);
            tempword = word;
            temp = items;
            word = (char **)calloc(WC,10*sizeof(char));
            items = assign_items(3,word);
            free(temp);
            free(tempword);


            post_menu(menu);
            box(menu_win,0,0);
            wrefresh(menu_win);
        }



    free(word);
    unpost_menu(menu);
    destroy_items(no,items);
    endwin();
    delscreen(screen);
    fclose(tty);

    return 0;
}
