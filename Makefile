
CC:=gcc
CFLAGS:=

OBJS:=main.o \
pass1.o \
pass2.o

TARGET:=sic_asm.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)
