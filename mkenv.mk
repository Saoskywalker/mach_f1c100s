.PHONY:all clean test_makefile
.PHONY:write mktool dump run

BUILD_VERBOSE = 0
ifeq ($(BUILD_VERBOSE),0)
Q = @
else
Q =
endif

# define main target
PROG ?= APP

# system app extension
EXTENSION ?= .elf

# finally file type
FINALLY_TYPE ?= .bin

# define output dir
BUILD ?= build

# define command
RM = rm
ECHO = @echo
CP = cp
MKDIR = mkdir
SED = sed
PYTHON = python

#gcc compiler select
CROSS_COMPILE ?= arm-eabi-
CC	          = $(CROSS_COMPILE)gcc
AS	          = $(CROSS_COMPILE)gcc -x assembler-with-cpp
CXX 		  = $(CROSS_COMPILE)g++
GDB 		  = $(CROSS_COMPILE)gdb
LD	          = $(CROSS_COMPILE)ld
OBJCOPY	      = $(CROSS_COMPILE)objcopy
OBJDUMP       = $(CROSS_COMPILE)objdump
SIZE 		  = $(CROSS_COMPILE)size
STRIP 		  = $(CROSS_COMPILE)strip
AR 			  = $(CROSS_COMPILE)ar

#define
DEFINES	+= -D__ARM32_ARCH__=5 -D__ARM926EJS__

# compiler set
# Tune for Debugging or Optimization
DEBUG ?= 0
ifeq ($(DEBUG), 1)
PROG = main_debug
ASFLAGS		= -g -ggdb -Wall -O0 -ffreestanding -std=gnu99 $(DEFINES)
CFLAGS		= -g -ggdb -Wall -O0 -ffreestanding -std=gnu99 $(DEFINES)
CXXFLAGS	= -g -ggdb -Wall -O0 -ffreestanding -std=c++11 $(DEFINES)
else
PROG = main_release
ASFLAGS		= -Wall -Os -ffreestanding -std=gnu99 $(DEFINES)
CFLAGS		= -Wall -Os -ffreestanding -std=gnu99 $(DEFINES)
CXXFLAGS	= -Wall -Os -ffreestanding -std=c++11 $(DEFINES)
endif

LDFLAGS		+= -T arch/$(ARCH)/$(MACH)/f1c100s.ld -nostdlib
MCFLAGS		+= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork

#include path
INCDIRS	+= \
	-Istd_lib/include \
	-Iarch/arm32/public/include \
	-Iarch/arm32/mach_f1c100s/driver/include \
	-Iarch/arm32/mach_f1c100s/include \
	-Iarch/arm32/mach_f1c100s/include/f1c100s \

#library path
LIBDIRS	+=

#library
LIBS += -lgcc

#c source path
SRCDIRS_C += 

#base drivers src
SRC_C += \
	arch/arm32/public/lib/arm32.c \
	arch/arm32/mach_f1c100s/sys-clock.c \
	arch/arm32/mach_f1c100s/sys-dram.c \
	arch/arm32/mach_f1c100s/sys-uart.c \
	arch/arm32/mach_f1c100s/sys-copyself.c \
	arch/arm32/mach_f1c100s/sys-spi-flash.c \
	arch/arm32/mach_f1c100s/sys-mmu.c \
	arch/arm32/mach_f1c100s/exception.c \

# drivers src
SRC_C += \
	arch/arm32/mach_f1c100s/driver/gpio-f1c100s.c \
	arch/arm32/mach_f1c100s/driver/pwm-f1c100s.c \
	arch/arm32/mach_f1c100s/driver/reset-f1c100s.c \
	arch/arm32/mach_f1c100s/driver/clk-f1c100s-pll.c \
	arch/arm32/mach_f1c100s/driver/fb-f1c100s.c \
	arch/arm32/mach_f1c100s/driver/irq_misc.c \
	arch/arm32/mach_f1c100s/driver/wdog-f1c100s.c \
	arch/arm32/mach_f1c100s/driver/uart-16550.c \
	arch/arm32/mach_f1c100s/driver/spi-f1c100s.c \
	arch/arm32/mach_f1c100s/driver/timer_f1c100s.c \
	arch/arm32/mach_f1c100s/driver/sys_TPAdc.c \
	arch/arm32/mach_f1c100s/driver/sys_sd.c \
	arch/arm32/mach_f1c100s/driver/sys_tvd.c \
	arch/arm32/mach_f1c100s/driver/sys_defe.c \
	arch/arm32/mach_f1c100s/driver/sys_audio.c \
	arch/arm32/mach_f1c100s/driver/usb_phy.c \

# C standard library src
SRC_C += \
	std_lib/malloc.c \
	std_lib/stdio/printf.c \
	std_lib/stdlib/strntoumax.c \
	std_lib/stdlib/strtod.c \
	std_lib/stdlib/rand.c \
	std_lib/stdio/sscanf.c \
	std_lib/stdio/vsscanf.c \
	std_lib/string/strcpy.c \
	std_lib/string/strcat.c \
	std_lib/string/strcmp.c \
	std_lib/string/strncmp.c \
	std_lib/string/strlen.c \
	std_lib/string/strstr.c \
	std_lib/string/strrchr.c \
	std_lib/string/strcpy.c \
	std_lib/string/memcmp.c \
	std_lib/math/floor.c \
	std_lib/math/ceil.c \
	std_lib/math/fabs.c \
	std_lib/math/fmin.c \
	std_lib/ctype/tolower.c \
	std_lib/ctype/isupper.c \
	std_lib/ctype/isspace.c \
	std_lib/ctype/isdigit.c \

#asm source files
SRC_ASM += \
	arch/arm32/mach_f1c100s/start.S \
	arch/arm32/public/lib/memcpy.S \
	arch/arm32/public/lib/memset.S \

all: $(BUILD)/$(addprefix $(PROG), $(FINALLY_TYPE))
	$(ECHO) "<><><><><><>><><>><><><><><><><><><>><><><><><><>"
	$(ECHO) make compile ok: $< 
	$(Q)$(CP) $(BUILD)/$(addprefix $(PROG), $(FINALLY_TYPE)) arch/$(ARCH)/$(MACH)/tools/$(addprefix $(PROG), $(FINALLY_TYPE))
	$(ECHO) $(shell date +%F%n%T)
	$(ECHO) "<><><><><><>><><>><><><><><><><><><><><><><<><><>"

run:
	$(ECHO) run ./$(BUILD)/$(addprefix $(PROG), $(EXTENSION))
	@ ./$(BUILD)/$(addprefix $(PROG), $(EXTENSION))

clean:
	find ./$(BUILD)/ -name "*.o"  | xargs rm -f  
	find ./$(BUILD)/ -name "*.bin"  | xargs rm -f
	find ./$(BUILD)/ -name "*.elf"  | xargs rm -f
	find ./$(BUILD)/ -name "*.P"  | xargs rm -f
	find ./$(BUILD)/ -name "*.d"  | xargs rm -f
	find ./$(BUILD)/ -name "*.map"  | xargs rm -f

dump:
	$(OBJDUMP) -S myboot.o | less

write:
	sudo sunxi-fel -p spiflash-write 0 $(BUILD)/$(addprefix $(PROG), .bin)

mktool:
	cd tools/mksunxiboot && make
	cd arch/$(ARCH)/$(MACH)/tools/mksunxi && make

PACK_TOOL := arch/$(ARCH)/$(MACH)/tools/mksunxi/mksunxi

mkboot:
	$(ECHO) Make header information for brom booting
	@$(PACK_TOOL) $(BUILD)/$(addprefix $(PROG), .bin)

test_makefile:
	@echo $(SRC_C)
