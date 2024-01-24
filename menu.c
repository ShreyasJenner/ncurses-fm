#define wl 10
#define wc 100

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
    ITEM** items = (ITEM **)calloc(wc,sizeof(ITEM *));

    char name[no][3];

    for(i=0;i<no;i++) {
        sprintf(name[i],"%d",i);
        items[i] = new_item(name[i],"");
        //mvwprintw(menu_win,10+i,10,"%s",word[i]);
    }
    items[no] = NULL;

    return items;
}

void modify_items(int no, ITEM **item, char **descrip) {
    int i;
    for(i=0;i<no;i++) {
        
        FILE *ptr = fopen("/dev/pts/4","w");
        fprintf(ptr,"%s %d\n","inside loop",i);
        fclose(ptr);

        item[i]->description.str = descrip[i];
        item[i]->description.length = strlen(descrip[i]);
    }
}

void destroy_items(int no,ITEM **item) {
    int i;
    for(i=0;i<no;i++)
        free_item(item[i]);
}

void extra_handling(ITEM **items, int prev, int curr) {
    int i;
    if(prev>curr) {
        for(i=prev;i<curr;i++)
            free_item(items[i]);
        items[prev] = NULL;
    } else {
        char name[curr-prev+1][3];

        for(i=0;i<curr;i++) {
            sprintf(name[i],"%d",prev++);
            items[prev-1] = new_item(name[i],"");
            //mvwprintw(menu_win,10+i,10,"%s",word[i]);
        }
        items[curr] = NULL;

    }
}


int main() {

    ITEM **items,**new,**temp;
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

        no = word_count();
        prev = no;

        char **word;

        word = create_words(wc,10);

        store_words(word);

        items = assign_items(no);
        modify_items(no,items,word);

        menu = new_menu((ITEM **)items);

        set_menu_win(menu, menu_win);
        set_menu_sub(menu, menu_sub_win);

        box(menu_win, 0,0);

        post_menu(menu);
        wrefresh(menu_win);

        FILE *ptr;
    while((c=getchar())!='q') {
        no = word_count();

        if(prev!=no) {
            extra_handling(items,prev,no);
            prev = no;
        }
        unpost_menu(menu);

        display(menu_win,no,word);

        ptr = fopen("/dev/pts/4","w");
        fprintf(ptr,"%s %d\n","first",no);
        fclose(ptr);

        store_words(word);
        
        display(menu_win,no,word);

        ptr = fopen("/dev/pts/4","w");
        fprintf(ptr,"%s %d\n","second",no);
        fclose(ptr);

        modify_items(no,items,word);

        ptr = fopen("/dev/pts/4","w");
        fprintf(ptr,"%s %d\n","third",no);
        fclose(ptr);

        box(menu_win,0,0);
        doupdate();
        post_menu(menu);
        wrefresh(menu_win);
    }
    unpost_menu(menu);


    destroy_items(no, items);

    endwin();
    delscreen(screen);
    fclose(tty);

    return 0;
}
