#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 1000
#define MAX_COLUMNS 8

// Reads and manages the data from input.txt
void input_handler(int (*arr)[MAX_COLUMNS]) {
    const char* filename = "input1.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Reads the file line by line. Slices the line into multiple tokens by whitespace.
    char line[25];
    int count = 0;
    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        int col = 0;
        char *token = strtok(line, " \t");

        while(token != NULL){
            arr[row][col] = atoi(token);
            token = strtok(NULL, " \t");
            col++;
        }
        for(int i = col; i < MAX_COLUMNS; i++)
        {
            arr[row][i] = 0;
        }
        row++;
    }
    fclose(file);
}

int safe_reports(int (*arr)[MAX_COLUMNS]){
    int safe = 0;

    for(int rows = 0; rows < MAX_ROWS; rows++)
    {
        int is_increasing = 0;

        if(arr[rows][0] < arr[rows][1]){
            is_increasing = 1;
        }

        int is_safe = 1;

        for(int cols = 1; cols < MAX_COLUMNS; cols++)
        {
            if(arr[rows][cols]){
                if(is_increasing){
                    if(arr[rows][cols] <= arr[rows][cols-1] || arr[rows][cols] - arr[rows][cols-1] > 3){
                        is_safe = 0;
                    }
                }
                else{
                    if(arr[rows][cols-1] <= arr[rows][cols] || arr[rows][cols-1] - arr[rows][cols] > 3){
                        is_safe = 0;
                    }
                }
            }
        }
        safe += 1*is_safe;
    }
    return safe;
}

int is_increasing(int (*arr)[MAX_COLUMNS], int row) {
    int difference = 0;
    for(int col = 1; col < MAX_COLUMNS; col++){
        if(arr[row][col]){
            difference += arr[row][col] - arr[row][col-1];
        }
    }
    if(difference > 0)
        return 1;
    else
        return 0;
}

int dampened_reports(int (*arr)[MAX_COLUMNS]){
    int safe = 0;

    for(int rows = 0; rows < MAX_ROWS; rows++){
        int is_safe = 1;
        int is_dampened = 0;

        if(is_increasing(arr,rows)){
            for(int cols = 1; cols < MAX_COLUMNS && arr[rows][cols] != 0; cols++){
                if(arr[rows][cols] <= arr[rows][cols-1] || arr[rows][cols] - arr[rows][cols-1] > 3){
                        if(!is_dampened && arr[rows][cols+1]>arr[rows][cols-1] && arr[rows][cols+1] - arr[rows][cols-1] < 3 || arr[rows][cols+1] == 0){
                            is_dampened = 1;
                            cols++;
                        }
                        else
                            is_safe = 0;
                }
            }
        }
        else{
            for(int cols = 1; cols < MAX_COLUMNS && arr[rows][cols] != 0; cols++){
                if(arr[rows][cols-1] <= arr[rows][cols] || arr[rows][cols-1] - arr[rows][cols] > 3){
                        if(!is_dampened && arr[rows][cols-1] > arr[rows][cols+1] && arr[rows][cols-1] - arr[rows][cols+1] < 3 || arr[rows][cols+1] == 0){
                                is_dampened = 1;
                                cols++;
                        }
                        else
                            is_safe = 0;
                }
            }
        }
        safe += 1*is_safe;
    }
    return safe;
}

int main() {

    int reports[MAX_ROWS][MAX_COLUMNS];
    input_handler(reports);

    printf("Safe Reports: %d\n",safe_reports(reports));
    printf("Dampened Reports: %d\n",dampened_reports(reports));

    return 0;
}
