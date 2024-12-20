#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBERS 1000

// Reads and manages the data from input.txt
void input_handler() {
    const char* filename = "input.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Reads the file line by line. Slices the line into two tokens by whitespace.
    char line[16];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {

        char *token = strtok(line, " \t");
        int num = atoi(token);

        while(token != NULL){
        token = strtok(NULL, " \t");
        num = atoi(token);
        }
    }
    fclose(file);
}

int main() {
    input_handler();

    return 0;
}
