#ifndef PROC_INFO_H
#define PROC_INFO_H

#include <types.h>

#define PROC_INFO_CMD_MAX 256

typedef struct {
	int32_t pid; 
	int32_t father_pid;
	int32_t owner; 
	int32_t state; 
	uint32_t start_sec;
	uint32_t heap_size;
	char cmd[PROC_INFO_CMD_MAX];
} procinfo_t;
	
#endif
