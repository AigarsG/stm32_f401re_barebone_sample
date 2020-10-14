RM = rm -rf
GCC = $(CROSS_COMPILE)gcc 
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
CFLAGS = -pedantic-errors -std=c89 -Wpedantic
ifdef CROSS_COMPILE
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb
endif

# Debug version
ifdef DEBUG
CFLAGS += -g -O0
CFLAGS += -DDEBUG=1
endif

TARGET=main.bin
ELF=$(TARGET:.bin=.elf)
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(ELF)
	$(OBJCOPY) -O binary $^ $@

$(ELF): $(OBJ)
	$(LD) -Tstm32_f401re.ld $^ -o $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET) $(ELF) $(OBJ)
