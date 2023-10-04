
#include "main.h"

void get_input_string(char *str_in);

char mem_string[MAX_CHAT_STRING][MAX_CHAT_STR_LENGTH];


#define BUFLEN 32	//Max length of buffer

int main(void)
{
    int shm/*, len, cmd, mode = 0*/;
//    int mode = 0;
    char *addr;
    
//    mode = O_CREAT;
    
    if ( (shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT|O_RDWR, 0777)) == -1 ) {perror("shm_open");return 1;}
    if ( (ftruncate(shm, (MAX_CHAT_STRING*MAX_CHAT_STR_LENGTH))) == -1 ) {perror("ftruncate");return 1;}
    
    addr = mmap(0, (MAX_CHAT_STRING*MAX_CHAT_STR_LENGTH), PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    if ( addr == (char*)-1 )
    {
        perror("mmap");
        return 1;
    }
    
    
    
    sem_t *sem;
    if ( (sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 1)) == SEM_FAILED ) {
        perror("sem_open");
        return 1;
    }
//    sem_post(sem);
    
    
    memset(addr, 0, (MAX_CHAT_STRING*MAX_CHAT_STR_LENGTH));
    
    
/******************************************************************
int sval;
do
{
sem_getvalue(sem, &sval); // показывает текущее значение семафора
printf("sem_val= %d\n", sval);
if(0 < sval){sem_wait();}
}
while(0 != sval)
sem_post(sem);
/******************************************************************/
    int sval;
    sem_getvalue(sem, &sval); // показывает текущее значение семафора
    printf("sem_val= %d\n", sval);
    
    {
    char buf[BUFLEN];
    while(1)
    {
        
        get_input_string(buf);
        
        if (strcmp (buf, "exit")==0)
        {
            break;
        }
        else{printf("Error input\n");}
        
        
    }
    }
    
    if ( sem_close(sem) < 0 ){perror("sem_close");}
    sem_unlink(SEMAPHORE_NAME); 	// отсоединяемся от группы
    
    printf("The End\n");
    return 0;
}


void get_input_string(char *str_in)
{
    int ch = 0;
    int temp_count = 0;
    while ((ch = getchar()) != '\n')
    {
        (*(str_in + temp_count)) = ch;
        ++temp_count;
        if(temp_count > (BUFLEN - 2 ))
        {
//          (*(str_in + temp_count)) = '\n';
//          ++temp_count;
          break;
        }
    }
//    (*(str_in + temp_count)) = '\n';
    (*(str_in + temp_count)) = 0;    
}


