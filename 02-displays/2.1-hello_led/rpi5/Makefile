CC      = aarch64-linux-gnu-gcc
SYSROOT = $(HOME)/rpi5-sysroot
CFLAGS  = -Wall -Wextra -I$(SYSROOT)/include
LDFLAGS = -L$(SYSROOT)/lib
LIBS    = -lgpiod

TARGET  = work/main
SRC     = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIBS)

deploy: $(TARGET)
	scp $(TARGET) yalong@rpi5.local:~/

clean:
	rm -f $(TARGET)