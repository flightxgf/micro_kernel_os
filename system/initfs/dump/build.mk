DUMP_OBJS = initfs/dump/dump.o

DUMP = $(TARGET_DIR)/initfs/dump

PROGS += $(DUMP)
CLEAN += $(DUMP_OBJS)

$(DUMP): $(DUMP_OBJS) $(LIB_OBJS)
	$(LD) -Ttext=100 $(DUMP_OBJS) $(LIB_OBJS) -o $(DUMP) $(LDFLAGS)
	$(OBJDUMP) -D $(DUMP) > $(TARGET_DIR)/asm/dump.asm

