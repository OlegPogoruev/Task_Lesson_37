
#include "main.h"


char name_thread[MAX_LEN_NAME] = "User_";
void *threads_scan(void *param); /* stream function */


//char mem_string[MAX_CHAT_STRING][MAX_CHAT_STR_LENGTH];
char mem_string[MAX_CHAT_STRING*MAX_CHAT_STR_LENGTH];

void get_input_string(char *str_in);
void gluing_strings(char* str_com, char* str_1, char* str_2);
void parsed_log(char *str_in);


char nic_name[MAX_LEN_NIC_NAME];


struct st_control_threads control_threads;


/////////////////////////////////////
WINDOW *win_user;// Shows current directory left
WINDOW *win_chat;// Shows current directory right
WINDOW *type_win;// Shows current directory right

void init_windows(void);
WINDOW *create_newwin(int height, int width, int starty, int startx);
void refreshWindows(void);

int maxx, maxy;
/////////////////////////////////////

sem_t *sem;
char *addr;

int main(void)
{
    int shm/*, len, cmd, mode = 0*/;
    
    
    
    if ( (shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_RDWR, 0777)) == -1 ) {perror("shm_open");return 1;}
//    if ( (ftruncate(shm, (MAX_CHAT_STRING*MAX_CHAT_STR_LENGTH))) == -1 ) {perror("ftruncate");return 1;}
    
    addr = mmap(0, (MAX_CHAT_STRING*MAX_CHAT_STR_LENGTH), PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    if ( addr == (char*)-1 )
    {
        perror("mmap");
        return 1;
    }
    
    
    printf("Enter nic name (max 20 char):\n");
    {
        char temp_array[20];
        scanf("%20s", temp_array);
        sprintf (nic_name, "/%s_%d", temp_array, getpid());
//        printf("nic_name = %s\n", nic_name);
    }
    
    
/////////////////////////////////////////////////////////////////////////////////////////////////
    WINDOW * point_window;
    point_window = initscr();

    initscr();
//    noecho();
    curs_set(0);
    start_color();

    getmaxyx(stdscr, maxy, maxx);// Get Size of terminal
//    maxy = maxy - 2;// Save last two rows for status_win
    
    init_windows();// Make the two windows side-by-side and make the status window
    refreshWindows();// Draw borders and refresh windows
    
/////////////////////////////////////////////////////////////////////////////////////////////////
    wmove(type_win,1,1);
    
    
    
    if ( (sem = sem_open(SEMAPHORE_NAME, O_RDWR)) == SEM_FAILED ) {
        perror("sem_open");
        return 1;
    }
    
    
//////////////////////////////////////////////////////////////////////////////////////////////
	pthread_t tid_read;	/* thread ID */
	pthread_attr_t attr_read; /* thread attributes */
 
 	control_threads.status = 0;
	pthread_attr_init(&attr_read);							/* get default attribute values */
	if(0 != pthread_create(&tid_read, &attr_read, threads_scan, name_thread))	/* create a new thread */
 	{endwin();perror ("PTHREAD_CREATE");return (-1);}
 //////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    while(1)
    {
        char temp_array[MAX_LEN_STRING];
        char temp_string[MAX_LEN_STRING];
        long temp = 0;
        int temp2 = 0;
        get_input_string(temp_string);
        
        if (strcmp (temp_string, "exit\n")==0)
        {
            control_threads.status = 1;
            break;
        }
        
        
        gluing_strings(temp_array, nic_name, temp_string);
        
        
        sem_wait(sem);
        while(0 != (*(addr + temp)))
        {++temp;}
        
        while(0 != (*(temp_array + temp2)))
        {
            (*(addr + temp)) = (*(temp_array + temp2));
            ++temp;
            ++temp2;
            if(temp >= (MAX_CHAT_STRING*MAX_CHAT_STR_LENGTH)){break;}
        }
        (*(addr + temp)) = 0;
        sem_post(sem);
        

        wclear(type_win);
        box(type_win,0,0);
        wrefresh(type_win);
        wmove(type_win,1,1);

    }
    
    pthread_join(tid_read,NULL);		/* waiting for the thread to complete execution */
    endwin();
    printf("The End\n");
    return 0;
}



/* Control passes streaming function */
void *threads_scan(void *param) 
{
    while(0 == control_threads.status)
    {
        sem_wait(sem);
        memcpy(mem_string, addr, (MAX_CHAT_STRING*MAX_CHAT_STR_LENGTH));
        sem_post(sem);
        
        /*
        wclear(type_win);
        box(type_win,0,0);
        wrefresh(type_win);
        wmove(type_win,1,1);
        */
        wclear(win_chat);
        wmove(win_chat,1,0);
        wprintw(win_chat, "%s", mem_string);
        box(win_chat,0,0);
        wrefresh(win_chat);
        
        parsed_log(mem_string);
        
        
        sleep(1);
        
    }
    pthread_exit(0);
}

void parsed_log(char *str_in)
{
    char memo_name[MAX_USER][MAX_LEN_NIC_NAME];
    unsigned int mem_count = 0;
    char state_machin = 0;
    char temp_name[MAX_LEN_NIC_NAME];
    long temp = 0;
    char temp2 = 0;
    
    wclear(win_user);
    wmove(win_user,1,0);
    while(0 != (*(str_in + temp)))
    {
        
        if(0 == state_machin)
        {
            if(':' == (*(str_in + temp)))
            {
                temp_name[temp2] = '\n';
                temp_name[temp2 + 1] = 0;
                
                if(0 == mem_count)
                {
                    memcpy(memo_name[mem_count], temp_name, (temp2 + 2));
                    wprintw(win_user, "%s", temp_name);
                    ++mem_count;
                }
                else
                {
                    char flag_use_name = 0;
                    for(unsigned int i = 0; i < mem_count; ++i)
                    {
                        if (strcmp (memo_name[i], temp_name)==0)
                        {
                            flag_use_name = 1;
                            break;
                        }
                    }
                    if(0 == flag_use_name)
                    {
                        memcpy(memo_name[mem_count], temp_name, (temp2 + 2));
                        wprintw(win_user, "%s", temp_name);
                        ++mem_count;
                    }
                }
                
                temp2 = 0;
                ++state_machin;
            }
            else
            {
                temp_name[temp2] = (*(str_in + temp));
                ++temp2;
            }
        }
        
        if(1 == state_machin)
        {
            if('\n' == (*(str_in + temp)))
            {
                state_machin = 0;
            }
        }
        
        
        ++temp;
    }
    box(win_user,0,0);
    wrefresh(win_user);
    
}


void get_input_string(char *str_in)
{
    int ch = 0;
    int temp_count = 0;


    while ((ch = wgetch(type_win)) != '\n')
    {
    	addch(ch);
        (*(str_in + temp_count)) = ch;
        ++temp_count;
        if(temp_count > MAX_LEN_SCAN_STRING)
        {
          break;
        }
    }
    (*(str_in + temp_count)) = '\n';
    (*(str_in + temp_count + 1)) = 0;
}

/*
void get_input_string(char *str_in)
{
    int ch = 0;
    int temp_count = 0;
    while ((ch = getchar()) != '\n')
    {
        (*(str_in + temp_count)) = ch;
        ++temp_count;
        if(temp_count > (MAX_LEN_SCAN_STRING))
        {
//          (*(str_in + temp_count)) = '\n';
//          ++temp_count;
          break;
        }
    }
    (*(str_in + temp_count)) = '\n';
    (*(str_in + temp_count + 1)) = 0;    
}
*/

void gluing_strings(char* str_com, char* str_1, char* str_2)
{
    int count_com = 0;

    while(0 != (*(str_1 + count_com)))
    {
        (*(str_com + count_com)) = (*(str_1 + count_com));
        ++count_com;
        if((MAX_LEN_STRING - 1) == count_com){(*(str_com + count_com)) = 0;return;}
    }

///////////////////////////////////////////////////////////////////
    (*(str_com + count_com)) = ':';
    ++count_com;
    if((MAX_LEN_STRING - 1) == count_com){(*(str_com + count_com)) = 0;return;}

    (*(str_com + count_com)) = ' ';
    ++count_com;
    if((MAX_LEN_STRING - 1) == count_com){(*(str_com + count_com)) = 0;return;}
///////////////////////////////////////////////////////////////////

    int count_temp = 0;
    while(0 != (*(str_2 + count_temp)))
    {
        (*(str_com + count_com)) = (*(str_2 + count_temp));
        ++count_com;
        ++count_temp;
        if((MAX_LEN_STRING - 1) == count_com){(*(str_com + count_com)) = 0;return;}
    }

    (*(str_com + count_com)) = 0;

}



/*
   Creates current_win, preview_win and status_win
*/
void init_windows(void)
{
    win_user = create_newwin(maxy - 10*maxy/100 - 1, maxx/3, 0, 0);
    win_chat = create_newwin(maxy - 10*maxy/100 - 1, 2*maxx/3, 0, maxx/3);
    type_win = create_newwin(maxy - 90*maxy/100, maxx, 90*maxy/100, 0);
//    keypad(win_left, TRUE);
//    keypad(win_right, TRUE);
//    win_activ = win_left;
    wmove(win_user,1,0);
    wmove(win_chat,1,0);
    keypad(type_win, TRUE);
    wmove(type_win,1,1);
}


/*
   Creates a new window with dimensions `height` and `width` starting at `starty` and `startx`
*/
WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    return local_win;
}

/*
    Refresh ncurses windows
*/
void refreshWindows(void)
{
    box(win_user,0,0);
    box(win_chat,0,0);
    box(type_win,0,0);
    
    wrefresh(win_user);
    wrefresh(win_chat);
    wrefresh(type_win);
    wmove(type_win,1,1);
}




