#include <kernel/svc.h>
#include <kernel/schedule.h>
#include <kernel/system.h>
#include <kernel/proc.h>
#include <vfs.h>
#include <syscalls.h>
#include <kstring.h>
#include <kprintf.h>

static void sys_exit(context_t* ctx, int32_t res) {
	if(_current_proc == NULL)
		return;
	proc_exit(ctx, _current_proc, res);
}

static int32_t sys_getpid(void) {
	if(_current_proc == NULL)
		return -1;
	return _current_proc->pid;
}

static void sys_sleep_on(context_t* ctx, uint32_t event) {
	proc_sleep_on(ctx, event);
}

static void sys_wakeup(uint32_t event) {
	proc_wakeup(event);
}

static void sys_uart_debug(const char* s) {
	printf("%s", s);
}

static int32_t sys_malloc(int32_t size) {
	return (int32_t)proc_malloc(size);
}

static void sys_free(int32_t p) {
	if(p == 0)
		return;
	proc_free((void*)p);
}

static int32_t sys_fork(context_t* ctx) {
	proc_t *proc = kfork();
	if(proc == NULL)
		return -1;

	memcpy(&proc->ctx, ctx, sizeof(context_t));
	ctx->gpr[0] = proc->pid;
	proc->ctx.gpr[0] = 0;
	proc_switch(ctx, proc);
	return 0;
}

static void sys_waitpid(context_t* ctx, int32_t pid) {
	proc_waitpid(ctx, pid);
}

static int32_t sys_vfs_get_info(const char* name, fsinfo_t* info) {
	vfs_node_t* node = vfs_get(vfs_root(), name);
	if(node == NULL)
		return -1;
	memcpy(info, &node->fsinfo, sizeof(fsinfo_t));
	return 0;
}

static int32_t sys_vfs_set_info(vfs_node_t* node, fsinfo_t* info) {
	if(node == NULL)
		return -1;
	memcpy(&node->fsinfo, info, sizeof(fsinfo_t));
	return 0;
}

static int32_t sys_vfs_add(vfs_node_t* node, fsinfo_t* info) {
	if(node == NULL || info == NULL)
		return -1;
	
	node = vfs_simple_add(node, info->name);
	if(node == NULL)
		return -1;

	memcpy(&node->fsinfo, info, sizeof(fsinfo_t));
	return 0;
}

static int32_t sys_vfs_del(vfs_node_t* node) {
	if(node == NULL)
		return -1;
	
	vfs_del(node);
	return 0;
}

void svc_handler(int32_t code, int32_t arg0, int32_t arg1, int32_t arg2, context_t* ctx, int32_t processor_mode) {
	(void)arg1;
	(void)arg2;
	(void)ctx;
	(void)processor_mode;
	
	__irq_disable();

	switch(code) {
	case SYS_UART_DEBUG: 
		sys_uart_debug((const char*)arg0);		
		return;
	case SYS_EXIT:
		sys_exit(ctx, arg0);
		return;
	case SYS_MALLOC:
		ctx->gpr[0] = sys_malloc(arg0);
		break;
	case SYS_FREE:
		sys_free(arg0);
		return;
	case SYS_GET_PID:
		ctx->gpr[0] = sys_getpid();
		break;
	case SYS_SLEEP_ON:
		sys_sleep_on(ctx, (uint32_t)arg0);
		return;
	case SYS_WAKEUP:
		sys_wakeup((uint32_t)arg0);
		return;
	case SYS_FORK:
		ctx->gpr[0] = sys_fork(ctx);
		return;
	case SYS_WAIT_PID:
		sys_waitpid(ctx, arg0);
		return;
	case SYS_SEND_MSG:
		ctx->gpr[0] = proc_send_msg(arg0, (void*)arg1, (uint32_t)arg2);
		return;
	case SYS_GET_MSG:
		ctx->gpr[0] = (uint32_t)proc_get_msg(ctx, (int32_t*)arg0, (uint32_t*)arg1, (uint8_t)arg2);
		return;
	case SYS_VFS_GET_INFO:
		ctx->gpr[0] = sys_vfs_get_info((const char*)arg0, (fsinfo_t*)arg1);
		return;
	case SYS_VFS_SET_INFO:
		ctx->gpr[0] = sys_vfs_set_info((vfs_node_t*)arg0, (fsinfo_t*)arg1);
		return;
	case SYS_VFS_ADD:
		ctx->gpr[0] = sys_vfs_add((vfs_node_t*)arg0, (fsinfo_t*)arg1);
		return;
	case SYS_VFS_DEL:
		ctx->gpr[0] = sys_vfs_del((vfs_node_t*)arg0);
		return;

	case SYS_YIELD: 
		schedule(ctx);
		return;
	}
}

