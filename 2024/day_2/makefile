# Compiler settings
CC_LINUX = gcc
CC_WINDOWS = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra -O2

# Output files
TARGET_LINUX = main.out
TARGET_WINDOWS = main.exe

# Default rule: Build both Linux and Windows executables, then run the Linux executable
all: linux windows run_linux

# Build Linux executable
linux: $(TARGET_LINUX)

$(TARGET_LINUX): main.c
	$(CC_LINUX) $(CFLAGS) -o $@ $^

# Build Windows executable
windows: $(TARGET_WINDOWS)

$(TARGET_WINDOWS): main.c
	$(CC_WINDOWS) $(CFLAGS) -o $@ $^

# Run the Linux executable
run_linux: $(TARGET_LINUX)
	./$(TARGET_LINUX)

# Clean up build files
clean:
	rm -f $(TARGET_LINUX) $(TARGET_WINDOWS)
