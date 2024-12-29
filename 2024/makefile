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

# Default: Compile for both platforms and run newly compiled Linux binaries
all: linux windows run_new

# Compile Linux executables
linux: $(TARGET_LINUX)

%.out: %.c
	$(CC_LINUX) $(CFLAGS) -o $@ $<
	@echo $@ >> $(NEWLY_COMPILED)

# Compile Windows executables
windows: $(TARGET_WINDOWS)

%.exe: %.c
	$(CC_WINDOWS) $(CFLAGS) -o $@ $<
	@echo $@ >> $(NEWLY_COMPILED)

# Run all newly compiled Linux executables without arguments
run_new:
	@echo "Running all newly compiled Linux binaries..."
	@if [ -f "$(NEWLY_COMPILED)" ]; then \
	    grep '\.out$$' $(NEWLY_COMPILED) | while read -r binary; do \
	        dir=$$(dirname $$binary); \
	        echo "Running $$binary in $$dir..."; \
	        (cd $$dir && ./$$(basename $$binary)); \
	    done; \
	    rm -f $(NEWLY_COMPILED); \
	else \
	    echo "No newly compiled binaries to run."; \
	fi

# Run a specific day's executable with custom arguments
run_day:
	@if [ -n "$(day)" ]; then \
	    dir=day_$(day); \
	    binary=$$dir/main.out; \
	    src=$$dir/main.c; \
	    if [ -f $$src ]; then \
	        echo "Compiling $$src..."; \
	        $(CC_LINUX) $(CFLAGS) -o $$binary $$src; \
	        echo "Running $$binary in $$dir with arguments: $(ARGS)..."; \
	        (cd $$dir && ./main.out $(ARGS)); \
	    else \
	        echo "Error: Source file $$src does not exist."; \
	        exit 1; \
	    fi; \
	else \
	    echo "Error: Specify a day using 'make day=N run_day', where N is the day number."; \
	    exit 1; \
	fi

# Clean all compiled files and marker file
clean:
	rm -f $(TARGET_LINUX) $(TARGET_WINDOWS) $(NEWLY_COMPILED)
