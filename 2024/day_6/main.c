#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 11

void input_handler(const char* filename, char board[][MAX]);
void print_board(const char board[][MAX]);

int main(int argc, char *argv[]) {
    const char *filename = "input.txt";
    char board[MAX][MAX] = {0};

    if (argc > 1) {
        filename = argv[1];
    }

    input_handler(filename,board);
    print_board(board);

    return 0;
}

void input_handler(const char* filename,char board[][MAX]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX+1];
    unsigned long index = 0;

    while (fgets(line, sizeof(line), file)) {
        if(index >= MAX) { printf("Error: Too many lines, buffer too small."); exit(EXIT_FAILURE); }
        //printf("%lu , %s\n",index,line);
        strcpy(line,board[index]);
        index++;
    }
    fclose(file);
}

void print_board(const char board[][MAX]) {
    unsigned long index = 0;
    while (index < MAX)
    {
        printf("%s",board[index]);
        index++;
    }
}
