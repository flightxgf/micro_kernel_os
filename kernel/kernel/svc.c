#include <kernel/svc.h>
#include <kernel/schedule.h>
#include <kernel/proc.h>
#include <syscalls.h>
#include <printk.h>

static int32_t sys_uart_debug(const char* s) {
	printk("%s", s);
	return 0;
}

static int32_t svc_handler_raw(int32_t code, int32_t arg0, int32_t arg1, int32_t arg2, context_t* ctx, int32_t processor_mode) {
	(void)arg1;
	(void)arg2;
	(void)ctx;
	(void)processor_mode;

	switch(code) {
	case SYS_UART_DEBUG: 
		return sys_uart_debug((const char*)arg0);		
	case SYS_YIELD: 
		schedule(ctx);
		return 0;
	}
	return -1;
}

void svc_handler(int32_t code, int32_t arg0, int32_t arg1, int32_t arg2, context_t* ctx, int32_t processor_mode) {
	ctx->gpr[0] = svc_handler_raw(code, arg0, arg1, arg2, ctx, processor_mode);
}
