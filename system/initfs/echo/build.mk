ECHO_OBJS = initfs/echo/echo.o

ECHO = $(TARGET_DIR)/initfs/echo

PROGS += $(ECHO)
CLEAN += $(ECHO_OBJS)

$(ECHO): $(ECHO_OBJS) $(LIB_OBJS)
	$(LD) -Ttext=100 $(ECHO_OBJS) $(LIB_OBJS) -o $(ECHO) $(LDFLAGS)
	$(OBJDUMP) -D $(ECHO) > $(TARGET_DIR)/asm/echo.asm

