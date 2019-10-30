#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

void *malloc(size_t size);
void free(void* ptr);
void exit(int status);
int execl(const char* fname, const char* arg, ...);

#endif
