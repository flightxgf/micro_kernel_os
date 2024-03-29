#ifndef UNISTD_H
#define UNISTD_H

#include <stdint.h>
#include <errno.h>

#define ERR_RETRY -2

int getpid(void);
int fork(void);
unsigned int sleep(unsigned int seconds);

int read(int fd, void* buf, uint32_t size);
int write(int fd, const void* buf, uint32_t size);

void exec_initfs(const char* fname);
int exec(const char* cmd_line);

char* getcwd(char* buf, uint32_t size);
int chdir(const char* path);

int dup2(int from, int to);
int dup(int from);

int pipe(int fds[2]);

#endif
