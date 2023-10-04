
//#include <stdlib.h>
//#include <sys/wait.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


#include <ncurses.h>

#include <pthread.h>


#include <semaphore.h>

#define SEMAPHORE_NAME "/my_semaphore"



#define SHARED_MEMORY_OBJECT_NAME "chat_shared_memory"

#define MAX_CHAT_STRING		8192
#define MAX_CHAT_STR_LENGTH	256



#define MAX_USER	       100
#define MAX_LEN_NIC_NAME	32

#define MAX_LEN_NAME 32
#define MAX_LEN_STRING	256


#define MAX_LEN_SCAN_STRING	(MAX_LEN_STRING - MAX_LEN_NIC_NAME - 4)


struct st_control_threads{
//    char array_name[MAX_LEN_NAME];
//    unsigned int msg_prio;
    int status;
};




