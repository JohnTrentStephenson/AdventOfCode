#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEARCH "XMAS"
#define DIRECTIONS {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}}
#define XSEARCH "MAS"

typedef struct{
    int row;
    int col;
} Coordinate;


int debug = 0;

// Reads and manages the data from the given file
void input_handler(const char* filename, char ***grid, int *rows, int *cols){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int max_rows = 0;
    int max_cols = 0;
    char buffer[1024];

    while(fgets(buffer, sizeof(buffer), file)){
        int length = strlen(buffer);
        if(buffer[length - 1] == '\n'){
            buffer[length - 1] = '\0';
            length--;
        }
        if(!max_cols) max_cols = length;
        else if(length != max_cols){
            fprintf(stderr, "Error: Input file can not contain rows of varying length.\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        max_rows++;
    }

    *rows = max_rows;
    *cols = max_cols;

    *grid = (char **)malloc(max_rows * sizeof(char *));
    if(*grid == NULL) {
        perror("Error allocating memory for grid");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < max_rows; i++){
        (*grid)[i] = (char *)malloc(max_cols * sizeof(char));
        if((*grid)[i] == NULL){
            perror("Error allocating memory for grid row");
            for(int j = 0; j < i; j++) {
                free((*grid)[j]);
            }
            free(*grid);
            exit(EXIT_FAILURE);
        }
    }

    rewind(file);

    int row = 0;
    while(fgets(buffer, sizeof(buffer), file)){
        memcpy((*grid)[row], buffer, max_cols);
        row++;
    }
    fclose(file);
}

char get_char(int row,int col, char **grid, int rows, int cols){
    if(row >= 0 && row < rows && col >=0 && col < cols) return grid[row][col]; else return'#';
}

void print_grid(char **grid, int rows, int cols){
    for (int row = 0; row < rows; row++)
    {
        printf("Line %d:\t", row+1);
        for(int col = 0; col < cols; col++)
        {
            printf("%c",get_char(row,col,grid,rows,cols));
        }
        printf("\n");
    }
}

int find_potential_matches(char **grid, char search, int rows, int cols){
    int potentials = 0;
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            if(get_char(row,col,grid,rows,cols) == search) potentials++;
        }
    }
    return potentials;
}

Coordinate *allocate_coordinate_array(int count){
    Coordinate * coordinates = (Coordinate *)malloc(count * sizeof(Coordinate));
    if(coordinates == NULL){
        perror("Error allocating memory for coordinates");
        exit(EXIT_FAILURE);
    }
    return coordinates;
}

void fill_coordinate_array(char **grid, Coordinate *coordinates, char search, int rows, int cols)
{
    int index= 0;

    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col ++){
            if(get_char(row,col,grid,rows,cols) == search){
                coordinates[index].row = row;
                coordinates[index].col = col;
                index++;
            }
        }
    }
}

int find_full_matches(char **grid, const Coordinate *coordinates, const char* target, int count, int rows, int cols){
    int matches = 0;
    int length = strlen(target);
    Coordinate directions[8] = DIRECTIONS;
    for(int potential_matches = 0; potential_matches < count; potential_matches++){
        for(int direction = 0; direction < 8; direction++){

            int final_row = coordinates[potential_matches].row + directions[direction].row*(length-1);
            int final_col = coordinates[potential_matches].col + directions[direction].col*(length-1);
            if(get_char(final_row,final_col,grid,rows,cols) != target[length-1]) continue;

            int valid_match = 1;

            for(int index = 1; index < length; index++){
                int new_row = coordinates[potential_matches].row + directions[direction].row*index;
                int new_col = coordinates[potential_matches].col + directions[direction].col*index;

                if(get_char(new_row, new_col, grid, rows,cols) != target[index]){
                    valid_match = 0;
                    break;
                }
            }
            if (valid_match) matches++;
        }
    }
    return matches;
}

int is_valid_MAS(char **grid, int start_row, int start_col, int row_dir, int col_dir, int rows, int cols){
    for(int i = 0; i < 3; i++)
    {
        char new_char = get_char(start_row + row_dir * i, start_col + col_dir * i, grid, rows, cols);
        if( new_char != XSEARCH[i]){
            if(debug) printf("Match invalid %c does not equal %c\n", new_char, XSEARCH[i]);
            return 0;
        }
    }
    return 1;
}

int sum_valid_pivot(char **grid, int pivot_row, int pivot_col, int rows, int cols){

    Coordinate directions[8] = DIRECTIONS;

    for(int direction = 0; direction < 4; direction++){
        if(debug) printf("Pivot at (%d, %d): Checking direction %d\n", pivot_row, pivot_col, direction);
        int row_dir = directions[direction].row;
        int col_dir = directions[direction].col;

        int start_row = pivot_row - row_dir;
        int start_col = pivot_col - col_dir;

        int opposite = (direction + 4) % 8;
        int start_row_opp = pivot_row - directions[opposite].row;
        int start_col_opp = pivot_col - directions[opposite].col;

        int original_or_opposite_valid =
            is_valid_MAS(grid, start_row, start_col, row_dir, col_dir, rows, cols) ||
            is_valid_MAS(grid, start_row_opp, start_col_opp, directions[opposite].row, directions[opposite].col, rows, cols);

        if(debug) printf("Original arm valid: %d, Opposite arm valid: %d\n", 
                  is_valid_MAS(grid, start_row, start_col, row_dir, col_dir, rows, cols), 
                  is_valid_MAS(grid, start_row_opp, start_col_opp, directions[opposite].row, directions[opposite].col, rows, cols));

        if(!original_or_opposite_valid){
            continue;
        }

        int rotated_cw = (direction + 2) % 8;
        int rotated_ccw = (direction -2 + 8) % 8;
        int start_row_cw = pivot_row - directions[rotated_cw].row;
        int start_col_cw = pivot_col - directions[rotated_cw].col;
        int start_row_ccw = pivot_row - directions[rotated_ccw].row;
        int start_col_ccw = pivot_col - directions[rotated_ccw].col;
        if(debug) printf("CW arm valid: %d, CCW arm valid: %d\n", 
                  is_valid_MAS(grid, start_row_cw, start_col_cw, directions[rotated_cw].row, directions[rotated_cw].col, rows, cols), 
                  is_valid_MAS(grid, start_row_ccw, start_col_ccw, directions[rotated_ccw].row, directions[rotated_ccw].col, rows, cols));

        if(is_valid_MAS(grid, start_row_cw, start_col_cw, directions[rotated_cw].row, directions[rotated_cw].col, rows, cols) || 
           is_valid_MAS(grid, start_row_ccw, start_col_ccw, directions[rotated_ccw].row, directions[rotated_ccw].col, rows, cols)){
            if(debug) printf("Valid Xmas found %c %d, %d\n", grid[pivot_row][pivot_col], pivot_row,pivot_col);
            return 1;
        }
    }
    return 0;
}

int count_valid_pivots(char **grid, int rows, int cols){
    int matches = 0;
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            if(get_char(row, col, grid, rows, cols) == 'A' && sum_valid_pivot(grid,row,col,rows,cols)){
                if(debug) printf("Pivot %c found at (%d, %d)\n",get_char(row, col, grid, rows, cols),row,col);
                matches++;
            }
        }
    }
    return matches;
}

int main(int argc, char *argv[]) {
    const char *filename = "input.txt";
    const char *target = SEARCH;

    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--debug") == 0){ // Check if a debug argument is passed.
            debug = 1;
        } else if (strcmp(argv[i], "--file") == 0 && i + 1 < argc){ //Check if a file argument is passed.
            filename = argv[++i];
        } else if (strcmp(argv[i], "--target") == 0 && i + 1 < argc){ //Check if a target argument is passed.
            target = argv[++i];
        } else {
            fprintf(stderr, "Unkown argument: %s\n", argv[i]); //If an improprer argument is pased exit out.
            return EXIT_FAILURE;
        }
    }
    //Make sure the search target is not empty
    if(target[0]=='\0') {
        fprintf(stderr, "Error: Search target cannot be empty.\n");
        exit(EXIT_FAILURE);
    }

    char **grid = NULL;
    int rows = 0, cols = 0;

    input_handler(filename, &grid, &rows, &cols);
    int potential_matches = find_potential_matches(grid,target[0],rows,cols);
    Coordinate *coordinates = allocate_coordinate_array(potential_matches);
    fill_coordinate_array(grid,coordinates,target[0],rows,cols);

    if(debug){
        printf("---->BEGINING OF DEBUG OUTPUT<----\n");
        print_grid(grid, rows, cols);

        for(int i = 0;i < potential_matches; i++)
        {
            printf("Starting Point %d: (%d, %d)\n",i+1,coordinates[i].row,coordinates[i].col);
        }

        printf("Grid size: %dx%d\n",rows,cols);
        printf("Found %d potential matches\n", potential_matches);
        printf("---->END OF DEBUG OUTPUT<----\n");
    }

    printf("Found %d full matches\n", find_full_matches(grid, coordinates, target, potential_matches, rows, cols));
    printf("Found %d X MAS's\n", count_valid_pivots(grid, rows, cols) );

    for(int i = 0; i < rows; i++){
        free(grid[i]);
    }
    free(grid);
    free(coordinates);

    return 0;
}
