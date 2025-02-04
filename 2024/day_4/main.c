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

typedef struct{
    char **grid;
    int rows;
    int cols;
} GridData;

typedef struct{
    Coordinate *coordinates;
    int matches;
} MatchedCoords;

int debug = 0;

// Reads and manages the data from the given file
void input_handler(const char* filename, GridData *grid_data){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int max_rows = 0;
    int max_cols = 0;
    char buffer[1024];

    while(fgets(buffer, sizeof(buffer), file)) {
        int length = strlen(buffer);
        if(buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
            length--;
        }
        if(!max_cols) max_cols = length;
        else if(length != max_cols) {
            fprintf(stderr, "Error: Input file can not contain rows of varying length.\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        max_rows++;
    }

    grid_data->rows = max_rows;
    grid_data->cols = max_cols;
    grid_data->grid = (char **)malloc(max_rows * sizeof(char *));

    if(grid_data->grid == NULL) {
        perror("Error allocating memory for grid");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < max_rows; i++) {
        grid_data->grid[i] = (char *)malloc(max_cols * sizeof(char));
        if(grid_data->grid[i] == NULL) {
            perror("Error allocating memory for grid row");
            for(int j = 0; j < i; j++) {
                free(grid_data->grid[j]);
            }
            free(grid_data->grid);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    rewind(file);

    int row = 0;
    while(fgets(buffer, sizeof(buffer), file)){
        memcpy(grid_data->grid[row], buffer, max_cols);
        row++;
    }
    fclose(file);
}

char get_char(const GridData *grid_data, int row, int col){
    if(row >= 0 && row < grid_data->rows && col >=0 && col < grid_data->cols) return grid_data->grid[row][col]; else return'\0';
}

void print_grid(const GridData *grid_data){
    for (int row = 0; row < grid_data->rows; row++)
    {
        printf("Line %d: \t", row+1);
        for(int col = 0; col < grid_data->cols; col++)
        {
            printf("%c",get_char(grid_data,row,col));
        }
        printf("\n");
    }
}

int find_potential_matches(const GridData *grid_data, char search){
    int potentials = 0;
    for(int row = 0; row < grid_data->rows; row++){
        for(int col = 0; col < grid_data->cols; col++){
            if(get_char(grid_data,row,col) == search) potentials++;
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

void fill_coordinate_array(const GridData *grid_data, Coordinate *coordinates, char search)
{
    int index= 0;

    for(int row = 0; row < grid_data->rows; row++){
        for(int col = 0; col < grid_data->cols; col ++){
            if(get_char(grid_data,row,col) == search){
                coordinates[index].row = row;
                coordinates[index].col = col;
                index++;
            }
        }
    }
}

void create_match_coords(const GridData *grid_data, MatchedCoords *matched_coords,const char search){
    int matches = find_potential_matches(grid_data, search);
    Coordinate *coordinates = allocate_coordinate_array(matches);

    matched_coords->coordinates = coordinates;
    matched_coords->matches = matches;

    fill_coordinate_array(grid_data, matched_coords->coordinates, search);

    if(debug){
       for(int i = 0;i < matched_coords->matches; i++)
        {
            printf("Starting Point %d: (%d, %d)\n",i+1,matched_coords->coordinates[i].row,matched_coords->coordinates[i].col);
        }

    }
}

int is_valid_mas(const GridData *grid_data, int start_row, int start_col, int row_dir, int col_dir, const char* target){
    int length = strlen(target);
    for(int i = 0; i < length; i++)
    {
        char new_char = get_char(grid_data,start_row + row_dir * i, start_col + col_dir * i);
        if( new_char != target[i]){
            if(debug) printf("Match invalid %c does not equal %c\n", new_char, target[i]);
            return 0;
        }
    }
    return 1;
}

int find_full_matches(const GridData *grid_data, const MatchedCoords *matched_coords, const char* target){
    int matches = 0;
    Coordinate directions[8] = DIRECTIONS;
    for(int potential_matches = 0; potential_matches < matched_coords->matches; potential_matches++){
        for(int direction = 0; direction < 8; direction++){
            matches += is_valid_mas(grid_data,matched_coords->coordinates[potential_matches].row,matched_coords->coordinates[potential_matches].col,
                                    directions[direction].row,directions[direction].col,target);
        }
    }
    return matches;
}



int is_valid_xmas(const GridData *grid_data, int pivot_row, int pivot_col){

    if(debug) printf("Checking pivot %c at (%d , %d) \n", get_char(grid_data,pivot_row, pivot_col),pivot_row,pivot_col);

    Coordinate directions[8] = DIRECTIONS;

    Coordinate se_dir = directions[1];
    Coordinate nw_dir = directions[5];
    Coordinate ne_dir = directions[3];
    Coordinate sw_dir = directions[7];

    int se_row_coord = pivot_row - se_dir.row;
    int se_col_coord = pivot_col - se_dir.col;

    int nw_row_coord = pivot_row - nw_dir.row;
    int nw_col_coord = pivot_col - nw_dir.col;

    int se_valid = is_valid_mas(grid_data, se_row_coord, se_col_coord, se_dir.row, se_dir.col, XSEARCH);
    int nw_valid = is_valid_mas(grid_data, nw_row_coord, nw_col_coord, nw_dir.row, nw_dir.col, XSEARCH);

    if(se_valid || nw_valid) {
        if(debug) printf("South East MAS: %d \t North West MAS: %d \n", se_valid, nw_valid);

        int ne_row_coord = pivot_row - ne_dir.row;
        int ne_col_coord = pivot_col - ne_dir.col;

        int sw_row_coord = pivot_row - sw_dir.row;
        int sw_col_coord = pivot_col - sw_dir.col;

        int ne_valid = is_valid_mas(grid_data, ne_row_coord, ne_col_coord, ne_dir.row, ne_dir.col, XSEARCH);
        int sw_valid = is_valid_mas(grid_data, sw_row_coord, sw_col_coord, sw_dir.row, sw_dir.col, XSEARCH);

        if(debug) printf("North East MAS %d \t South West MAS %d \n", ne_valid, sw_valid);

        if(ne_valid || sw_valid) {
            if (debug) printf("Valid X Mas found for (%d , %d) \n", pivot_row, pivot_col);
            return 1;
        }
    }

    return 0;
}

int count_valid_xmas(const GridData *grid_data, const MatchedCoords *matched_coords){
    int matches = 0;
    for(int potential_matches = 0; potential_matches < matched_coords->matches; potential_matches++) { 
            if(is_valid_xmas(grid_data,matched_coords->coordinates[potential_matches].row,matched_coords->coordinates[potential_matches].col)){
                matches++;
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
    if(debug) printf("---->BEGINING OF DEBUG OUTPUT<----\n"); 
    //Make sure the search target is not empty
    if(target[0]=='\0') {
        fprintf(stderr, "Error: Search target cannot be empty.\n");
        exit(EXIT_FAILURE);
    }
    GridData grid_data;

    input_handler(filename, &grid_data);



 
    MatchedCoords part1_coords;
    create_match_coords(&grid_data,&part1_coords, target[0]);

    if(debug) {
        print_grid(&grid_data);
        printf("Grid size: %dx%d\n",grid_data.rows,grid_data.cols);
        printf("Found %d potential matches\n", part1_coords.matches);
        printf("---->END OF DEBUG OUTPUT<----\n");

    }

    printf("Found %d full matches\n", find_full_matches(&grid_data, &part1_coords, target));
    free(part1_coords.coordinates);
    MatchedCoords part2_coords;
    create_match_coords(&grid_data,&part2_coords,'A');
    printf("Found %d X MAS's\n", count_valid_xmas(&grid_data,&part2_coords));
    free(part2_coords.coordinates);
    for(int i = 0; i < grid_data.rows; i++){
        free(grid_data.grid[i]);
    }
    free(grid_data.grid);

    return 0;
}
