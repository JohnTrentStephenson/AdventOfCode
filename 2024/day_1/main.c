#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBERS 1000

// Reads and manages the data from input.txt
void input_handler(int *first_list, int *second_list ) {
    const char* filename = "input.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Reads the file line by line. Slices the line into two tokens by whitespace.
    // Converts the two string tokens to ints and assigns them to appropriate array.
    char line[16];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " \t");
        first_list[count] = atoi(token);
        token = strtok(NULL, " \t");
        second_list[count] = atoi(token);

        count++;
    }
    fclose(file);
}

// A simple sort that finds the min value of the whole array and assigns that to the first position.
// It then scans and finds the min value of the whole array minus the first position... and so on.
void sort(int *arr){
    for(int count = 0; count < MAX_NUMBERS; count++){

        int min = arr[count];
        int min_index = count;

        for(int i = count; i < MAX_NUMBERS; i++)
        {
            if(arr[i] < min)
            {
                min = arr[i];
                min_index = i;
            }
        }

        arr[min_index] = arr[count];
        arr[count] = min;
    }
}

// Compares the numbers from the first list to the second list, counts the times it repeats
// and adds the number * the amount of times it repeated to a score. Returning a final score.
int similarity_score(int *first_list, int *second_list){
    int score = 0;

    for(int i = 0; i < MAX_NUMBERS; i++){
        int count = 0;
        for(int j = 0; j < MAX_NUMBERS; j++){
            if(first_list[i] == second_list[j]){
                count++;
            }
        }
        score += first_list[i]*count;
    }

    return score;
}

// Calculates the difference between a list of two numbers by iterating through both list
// and taking the absolute value of their difference.
int difference_score(int *first_list, int *second_list){
    int difference = 0;

    for(int i = 0; i < MAX_NUMBERS; i++){
        difference += abs(first_list[i] - second_list[i]);
    }
    return difference;
}

int main() {
    int first_list[MAX_NUMBERS];
    int second_list[MAX_NUMBERS];

    input_handler(first_list, second_list);

    sort(first_list);
    sort(second_list);

    printf("The difference for part 1: %d\n",difference_score(first_list,second_list));

    printf("The similarity for part 2: %d", similarity_score(first_list, second_list));

    return 0;
}
