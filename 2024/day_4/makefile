# Compiler settings
CC_LINUX = gcc
CC_WINDOWS = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra -O2

# Output files
TARGET_LINUX = main.out
TARGET_WINDOWS = main.exe

# Default rule: Build for both platforms and run the Linux version
all: linux windows run_linux

# Build Linux executable
linux: $(TARGET_LINUX)

$(TARGET_LINUX): main.c
	$(CC_LINUX) $(CFLAGS) -o $@ $^

# Build Windows executable
windows: $(TARGET_WINDOWS)

$(TARGET_WINDOWS): main.c
	$(CC_WINDOWS) $(CFLAGS) -o $@ $^

# Run the Linux executable with or without an input file
run_linux: $(TARGET_LINUX)
	@if [ -n "$(file)" ]; then \
	    ./$(TARGET_LINUX) $(file); \
	else \
	    ./$(TARGET_LINUX); \
	fi

# Clean up build files
clean:
	rm -f $(TARGET_LINUX) $(TARGET_WINDOWS)
