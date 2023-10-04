
//#include <stdlib.h>
//#include <sys/wait.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include <semaphore.h>

#define SEMAPHORE_NAME "/my_semaphore"


#define SHARED_MEMORY_OBJECT_NAME "chat_shared_memory"

#define MAX_CHAT_STRING		8192
#define MAX_CHAT_STR_LENGTH	256







