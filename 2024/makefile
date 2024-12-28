# Compiler settings
CC_LINUX = gcc
CC_WINDOWS = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra -O2

# Directories and Files
SRC_FILES := $(wildcard day_*/main.c)
TARGET_LINUX := $(SRC_FILES:.c=.out)
TARGET_WINDOWS := $(SRC_FILES:.c=.exe)

# Temporary marker file to track newly compiled targets
NEWLY_COMPILED = .newly_compiled

# Optional input file argument
file ?= input.txt

# Default rule: Build for both platforms and run newly compiled Linux executables
all: linux windows run_all

# Build Linux executables
linux: $(TARGET_LINUX)

%.out: %.c
	$(CC_LINUX) $(CFLAGS) -o $@ $<
	@echo $@ >> $(NEWLY_COMPILED)

# Build Windows executables
windows: $(TARGET_WINDOWS)

%.exe: %.c
	$(CC_WINDOWS) $(CFLAGS) -o $@ $<
	@echo $@ >> $(NEWLY_COMPILED)

# Run newly compiled Linux executables with optional file argument
run_all: linux
	@if [ -s $(NEWLY_COMPILED) ]; then \
	    for binary in `cat $(NEWLY_COMPILED)`; do \
	        dir=$$(dirname $$binary); \
	        echo "Running $$binary in $$dir with file=$(file)..."; \
	        (cd $$dir && ./$$(basename $$binary) $(file)); \
	    done; \
	    rm -f $(NEWLY_COMPILED); \
	else \
	    echo "No newly compiled binaries to run."; \
	fi

# Rebuild and run a specific day with optional file argument
run_day:
	@if [ -n "$(day)" ]; then \
	    dir=$(day); \
	    binary=$$dir/main.out; \
	    src=$$dir/main.c; \
	    if [ -f $$src ]; then \
	        echo "Compiling $$src..."; \
	        $(CC_LINUX) $(CFLAGS) -o $$binary $$src; \
	        echo "Running $$binary in $$dir with file=$(file)..."; \
	        (cd $$dir && ./main.out $(file)); \
	    else \
	        echo "Error: $$src does not exist."; \
	        exit 1; \
	    fi; \
	else \
	    echo "Error: Specify a day using 'make day=day_X run_day'."; \
	    exit 1; \
	fi

# Clean up all build files and the marker file
clean:
	rm -f $(TARGET_LINUX) $(TARGET_WINDOWS) $(NEWLY_COMPILED)
