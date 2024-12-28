#include <stdio.h>
#include <stdlib.h>

#define MAX 6

// Reads and manages the data from the given file
void input_handler(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX];

    while (fgets(line, sizeof(line), file)) {
        // Process the line (currently does nothing)
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    // Default filename
    const char *filename = "input.txt";

    // If a filename is provided as an argument, use it
    if (argc > 1) {
        filename = argv[1];
    }

    input_handler(filename);

    return 0;
}
