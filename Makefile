CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
CFLAGS = -pedantic-errors -std=c89 -Wpedantic
LDFLAGS = -Tstm32_f401re.ld
ifdef CROSS_COMPILE
CFLAGS += -mcpu=cortex-m4 -mthumb
LDFLAGS += --specs=nosys.specs
LDFLAGS += -nostartfiles
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


all: $(TARGET) $(ELF)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

$(ELF): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET) $(ELF) $(OBJ)
