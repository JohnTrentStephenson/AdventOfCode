#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DIRECTIONS {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}}
#define PART1 "XMAS"
#define PART2 "MAS"
#define MAXDIMENSION 140

char grid_data[MAXDIMENSION][MAXDIMENSION];

#if defined(__WORDSIZE) && __WORDSIZE == 64/* Check for a 64 bit system (size_t is likely unsigned long long) */
    #define SIZE_T_STATIC "%llu"
    #define SIZE_T_DYNAMIC "%*llu"
    #define PRINT_SIZE_T(x) (unsigned long long)(x)
#else /* Assume 32-bit (size_t is likely unsigned long) */
    #define SIZE_T_STATIC "%lu"
    #define SIZE_T_DYNAMIC "%*lu"
    #define PRINT_SIZE_T(x) (unsigned long)(x)
#endif

typedef struct{
    size_t row;
    size_t col;
} Coordinate;

typedef struct{
    Coordinate *coordinates;
    size_t matches;
} MatchedCoords;

/* Declarations */
void fill_grid(const char* filename);
char get_char(const size_t row, const size_t col);
MatchedCoords matched_char_coords(const char search);
int is_word_match(const size_t start_row, const size_t start_col, const int row_dir, const int col_dir, const char* target);
int is_x_match(const size_t pivot_row, const size_t pivot_col, const char *target);
size_t count_word_matches(const MatchedCoords *matched_coords, const char *target);
size_t count_x_matches(const MatchedCoords *matched_coords, const char *target);
void print_grid();

/* Definitions */
void fill_grid(const char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[MAXDIMENSION+2];

    size_t rows_read;
    for(rows_read = 0; rows_read < MAXDIMENSION; rows_read++){
        if(fgets(buffer,MAXDIMENSION+2,file) == NULL){
            if(ferror(file)){
                perror("Error reading from file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        memcpy(grid_data[rows_read],buffer,sizeof(char)*MAXDIMENSION);
    }
    fclose(file);
}
/* Because we use an unsigned variable only upper bounds checking can be done. */
char get_char(const size_t row, const size_t col){
    if(row < MAXDIMENSION && col < MAXDIMENSION) return grid_data[row][col]; else return'\0';
}

MatchedCoords matched_char_coords(const char search){
    MatchedCoords matched_coords;
    matched_coords.coordinates = NULL;
    matched_coords.matches = 0;

    Coordinate buffer[MAXDIMENSION*MAXDIMENSION];
    size_t index = 0;

    size_t row,col;
    for(row = 0; row < MAXDIMENSION; row++){
        for(col = 0; col < MAXDIMENSION; col++){
            if(get_char(row,col) == search){
                buffer[index].row = row;
                buffer[index].col = col;
                index++;
            }
        }
    }
    /* Allocate and fill the coordinate array if there are matches */
    if(index > 0){
        if(!(matched_coords.coordinates = malloc(index * sizeof(Coordinate)))){ perror("Error allocating memory for coordinates"); }
        matched_coords.matches = index;
        for(index = 0; index < matched_coords.matches; index++){ matched_coords.coordinates[index] = buffer[index]; }
    }

    return matched_coords;
}

int is_word_match(const size_t start_row, const size_t start_col, const int row_dir, const int col_dir, const char* target){

    size_t length = strlen(target);

    /* Since the function is potentially subtracting from an unsigned number this logic checks if we have caused size_t to wrap.
     * If it has that means the function would have tried to access a cell outside the bounds of the grid.
     * If that happens the word is not valid and we can return 0.
     * Since a check for wrapping from 0 to Max was needed, wrapping to 0 from Max is also checked for.*/

    size_t final_row, final_col;
    final_row = (length -1) * row_dir + start_row;
    final_col = (length -1) * col_dir + start_col;
    if(row_dir < 0){ if(final_row > start_row) return 0; } else if(final_row < start_row) { return 0; }
    if(col_dir < 0){ if(final_col > start_col) return 0; } else if(final_col < start_col) { return 0; }

    size_t index;
    for(index = 0; index < length; index++){
        if(get_char(start_row + row_dir * index, start_col + col_dir * index) != target[index]){ return 0; }
    }

    return 1;
}

int is_x_match(const size_t pivot_row, const size_t pivot_col, const char *target){

    Coordinate directions[8] = DIRECTIONS;

    Coordinate se_dir = directions[1];
    Coordinate nw_dir = directions[5];
    Coordinate ne_dir = directions[3];
    Coordinate sw_dir = directions[7];

    size_t se_row_coord = pivot_row - se_dir.row;
    if(se_row_coord > pivot_row) { return 0; }
    size_t se_col_coord = pivot_col - se_dir.col;
    if(se_col_coord > pivot_col) { return 0; }


    size_t nw_row_coord = pivot_row - nw_dir.row;
    size_t nw_col_coord = pivot_col - nw_dir.col;
    /* NW is defined as (-1,-1) so subtracting is actually adding and no underflow check is needed */

    int se_valid = is_word_match(se_row_coord, se_col_coord, se_dir.row, se_dir.col, target);
    int nw_valid = is_word_match(nw_row_coord, nw_col_coord, nw_dir.row, nw_dir.col, target);

    if(se_valid || nw_valid){
        size_t ne_row_coord = pivot_row - ne_dir.row;
        if(ne_row_coord > pivot_row) { return 0; }
        size_t ne_col_coord = pivot_col - ne_dir.col;
        /* The same situation is encountered for any N col check*/

        size_t sw_row_coord = pivot_row - sw_dir.row;
        /* Or W row check */
        size_t sw_col_coord = pivot_col - sw_dir.col;
        if(sw_col_coord > pivot_col) { return 0; }

        int ne_valid = is_word_match(ne_row_coord, ne_col_coord, ne_dir.row, ne_dir.col, target);
        int sw_valid = is_word_match(sw_row_coord, sw_col_coord, sw_dir.row, sw_dir.col, target);

        if(ne_valid || sw_valid){
            return 1;
        }
    }
    return 0;
}

size_t count_word_matches(const MatchedCoords *matched_coords, const char *target){
    size_t matches, potential_matches, direction;
    matches = 0;
    Coordinate directions[8] = DIRECTIONS;
    for(potential_matches = 0; potential_matches < matched_coords->matches; potential_matches++){
        for(direction = 0; direction < 8; direction++){
            matches += is_word_match(matched_coords->coordinates[potential_matches].row,
                                    matched_coords->coordinates[potential_matches].col,
                                    directions[direction].row,directions[direction].col,target);
        }
    }
    return matches;
}

size_t count_x_matches(const MatchedCoords *matched_coords, const char *target){
    size_t matches, potential_matches;

    matches = 0;
    for(potential_matches = 0; potential_matches < matched_coords->matches; potential_matches++){
            if(is_x_match(matched_coords->coordinates[potential_matches].row,matched_coords->coordinates[potential_matches].col,target)){
                matches++;
            }
    }
    return matches;
}

int main(int argc, char *argv[]){
    const char *filename = "input.txt";
    int debug = 0;

    int arg;
    for (arg = 1; arg < argc; arg++){
        if (strcmp(argv[arg], "--debug") == 0){ /* Check if a debug argument is passed. */
            debug = 1;
        } else if (strcmp(argv[arg], "--file") == 0 && arg + 1 < argc){ /* Check if a file argument is passed. */
            filename = argv[++arg];
        } else {
            fprintf(stderr, "Unkown argument: %s\n", argv[arg]); /* If an improprer argument is pased exit out. */
            return EXIT_FAILURE;
        }
    }

    fill_grid(filename);

    MatchedCoords part1_coords = matched_char_coords(PART1[0]);
    printf("Found " SIZE_T_STATIC " full matches\n", PRINT_SIZE_T(count_word_matches(&part1_coords, PART1)));
    free(part1_coords.coordinates);

    MatchedCoords part2_coords = matched_char_coords(PART2[1]);
    printf("Found " SIZE_T_STATIC " X %s's\n", PRINT_SIZE_T(count_x_matches(&part2_coords, PART2)),PART2);
    free(part2_coords.coordinates);

    if(debug){
        printf("-------DEBUG--------\n");
        print_grid();
        printf("-------END-------\n");
    }
    return 0;
}
/* Functions only used in debug mode */
void print_grid(){
    size_t row,col;
    for (row = 0; row < MAXDIMENSION; row++){
        printf("Line " SIZE_T_DYNAMIC ": ", 3, PRINT_SIZE_T(row+1));
        for(col = 0; col < MAXDIMENSION; col++){
            printf("%c",get_char(row,col));
        }
        printf("\n");
    }
}

