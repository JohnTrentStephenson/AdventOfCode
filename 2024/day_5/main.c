#include <stdio.h>
#include <stdlib.h>

#define MAX 6


void input_handler(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX];

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    const char *filename = "input.txt";

    if (argc > 1) {
        filename = argv[1];
    }

    input_handler(filename);

    return 0;
}
