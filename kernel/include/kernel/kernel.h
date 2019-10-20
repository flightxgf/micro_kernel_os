#ifndef KERNEL_H
#define KERNEL_H

#include <mm/mmu.h>

extern char _kernel_start[];
extern char _kernel_end[];

extern void set_kernel_vm(page_dir_entry_t* vm);
#endif