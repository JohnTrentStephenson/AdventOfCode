#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 3090
#define MAX_NUMBER_LENGTH 3

// Reads and manages the data from input.txt
int input_handler(int enhanced) {
    const char* filename = "input.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int sum = 0;
    int mul = 1;

    while (fgets(line, sizeof(line), file)) {
        int line_length = strlen(line);
        for(int parser = 0; parser < line_length;){
            if(line[parser] == 'm' && line[parser+1]== 'u' 
                && line[parser+2]== 'l' && line[parser+3] == '(' ){

                parser+=4;

                char arr1[MAX_NUMBER_LENGTH+1] = {0};
                char arr2[MAX_NUMBER_LENGTH+1] = {0};
                int num1 = 0, num2 = 0;

                int j = 0;
                while(line[parser] >='0' && line[parser]<='9'
                && j < MAX_NUMBER_LENGTH){
                    arr1[j++] = line[parser++];
                }
                arr1[j] = '\0';
                if(j == 0){
                    continue;
                } else {
                    num1 = atoi(arr1);
                }

                if (line[parser] == ','){
                    parser++; 
                } else {
                    continue;
                }

                j = 0;
                while(line[parser] >= '0' && line[parser] <='9'
                && j < MAX_NUMBER_LENGTH){
                    arr2[j++] = line[parser++];
                }
                arr2[j] = '\0';

                if(j == 0){
                    continue;
                } else{
                    num2 = atoi(arr2);
                }

                if(line[parser] == ')'){
                    parser++;
                } else{
                    continue;
                }
                if(enhanced){
                    sum += num1*num2*mul;
                } else sum += num1 * num2;

            } else if (line[parser] == 'd' && line[parser+1] == 'o' && line[parser+2] == '(' && line[parser+3] == ')'){
                mul = 1;
                parser +=4;
            } else if (line[parser] == 'd' && line[parser+1] == 'o' && line[parser+2] == 'n' && line[parser+3] == '\''
                && line[parser+4] == 't' && line[parser+5] == '(' && line[parser+6] == ')'){
                mul = 0;
                parser += 6;
            } else {
                parser++;
            }
        }
    }
    return sum;
    fclose(file);
}

int main() {
    printf("Part 1: %d\n", input_handler(0));
    printf("Part 2: %d\n",input_handler(1));

    return 0;
}
