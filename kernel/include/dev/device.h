#ifndef KDEVICE_TYPE_H
#define KDEVICE_TYPE_H

#define DEV_SLEEP -2

enum {
	DEV_TYPE_CHAR = 0,
	DEV_TYPE_MEM
};

enum {
	DEV_NULL = 0,
	DEV_UART0,
	DEV_KEYB,
	DEV_MOUSE,
	DEV_FRAMEBUFFER,
	DEV_NUM
};

enum {
	DEV_OP_INFO = 0,
	DEV_OP_ON,
	DEV_OP_OFF
};


#endif
