ifeq ($(strip $(DEVKITARM)),)
$(error "请将DEVKITARM添加到环境变量")
endif

PREFIX = $(DEVKITARM)/bin/arm-none-eabi-

CC = $(PREFIX)gcc
LD = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump

# define options for compilation
CFLAGS = -Wall -Os -fno-builtin -march=armv6k -mthumb -fshort-wchar
CFLAGS += -Ilibctru

# define options for linkage. Prevent the inclusion of standard start
# code and libraries.
LDFLAGS = -fno-builtin -nostartfiles -nodefaultlibs -Wl,--use-blx
#LDFLAGS += -fno-builtin -nostartfiles -T ld.S -Wl,--use-blx -Wl,-pie


# define options for the objdump
DUMPFLAGS = -xdsS

# use variables to refer to init code in case it changes
OBJS = head.o main.o 

#
# define build targets
#
all: payload_10.bin

VER11: payload_11.bin

clean:
	rm -f *.o *.elf *.bin *.dump


# build s-record with init code and c files linked together
payload_10.bin: $(OBJS)
	$(LD) $(LDFLAGS) -T ld.S -o payload_10.elf $(OBJS) $(LIBS)
	$(OBJDUMP) $(DUMPFLAGS) payload_10.elf > payload_10.dump
	$(OBJCOPY) -O binary payload_10.elf payload_10.bin

payload_11.bin: $(OBJS)
	$(LD) $(LDFLAGS) -T ld_11.S -o payload_11.elf $(OBJS) $(LIBS)
	$(OBJDUMP) $(DUMPFLAGS) payload_11.elf > payload_11.dump
	$(OBJCOPY) -O binary payload_11.elf payload_11.bin

# handle compilation of C files
%.o:%.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -c $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@


