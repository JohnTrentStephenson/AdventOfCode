#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

typedef enum { FALSE, TRUE } bool;

#define MAX_RULES 1200
#define MAX_UPDATES 25
#define MAX_UPDATE_SETS 200
#define LINE_BUFFER 100

struct Rule { unsigned long precedes; unsigned long follows; };
struct RuleSet { struct Rule rules[MAX_RULES]; size_t count; };
struct UpdateSet { unsigned long update[MAX_UPDATES]; size_t updates; };
struct UpdateList { struct UpdateSet update_set[MAX_UPDATE_SETS]; size_t sets; };


void input_handler(const char* filename, struct RuleSet *page_rules, struct UpdateList *update_list);
void print_input(const struct RuleSet *page_rules, const struct UpdateList *update_list);
bool valid_update_set(const struct RuleSet *page_rules, const struct RuleSet *temp_rules);
void construct_temp_ruleset(const struct UpdateSet *update_set, struct RuleSet *temp_rules);
unsigned long calc_result(const struct UpdateList *update_list, const struct RuleSet *page_rules);
struct Rule find_invalid_rule(const struct RuleSet *page_rules, const struct RuleSet *temp_rules);
unsigned long get_update_index(const struct UpdateSet *update_rules, const unsigned long update);
unsigned long correct_update_set(const struct RuleSet *page_rules, struct UpdateSet *invalid_set);
unsigned long calc_result_of_fixed(const struct RuleSet *page_rules, struct UpdateList *update_list);

int main(int argc, char *argv[]) {
    struct RuleSet page_rules = {0};
    struct UpdateList update_list = {0};

    const char *filename = "input.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    input_handler(filename, &page_rules, &update_list);
    printf("Part 1: %lu\n",calc_result(&update_list, &page_rules));
    printf("Part 2: %lu\n", calc_result_of_fixed(&page_rules,&update_list));

    return 0;
}


unsigned long calc_result_of_fixed(const struct RuleSet *page_rules, struct UpdateList *update_list) {
    unsigned long update_index;
    unsigned long sum = 0;

    for(update_index = 0; update_index < update_list->sets; update_index++) {
        struct RuleSet temp_rules = {0};
        struct UpdateSet *current_update = &update_list->update_set[update_index];

        construct_temp_ruleset(current_update,&temp_rules);
        if(!valid_update_set(page_rules,&temp_rules)) {
            sum += correct_update_set(page_rules, current_update);
        }
    }
    return sum;
}

unsigned long correct_update_set(const struct RuleSet *page_rules, struct UpdateSet *invalid_set) {
    struct RuleSet temp_rules = {0};
    unsigned long max_iterations = MAX_RULES * 2;
    unsigned long iterations = 0;

    construct_temp_ruleset(invalid_set, &temp_rules);
    while(!valid_update_set(page_rules, &temp_rules) && iterations <= max_iterations) {
        struct Rule temp_rule = find_invalid_rule(page_rules, &temp_rules);
        printf("[%lu, %lu] - ",temp_rule.precedes, temp_rule.follows);
        unsigned long invalid_follows = get_update_index(invalid_set, temp_rule.follows);
        unsigned long invalid_precedes = get_update_index(invalid_set, temp_rule.precedes);
        if(invalid_follows == ULONG_MAX || invalid_precedes == ULONG_MAX) {
            printf("Error: Index of rule [%lu, %lu] not found\n", temp_rule.precedes, temp_rule.follows);
            exit(EXIT_FAILURE);
        }
        printf("%lu, %lu\n", invalid_precedes, invalid_follows);
        invalid_set->update[invalid_follows] = temp_rule.precedes;
        invalid_set->update[invalid_precedes] = temp_rule.follows;
        construct_temp_ruleset(invalid_set, &temp_rules);
        iterations++;
    }
    unsigned long middle_index = invalid_set->updates/2;
    return invalid_set->update[middle_index];
}

unsigned long get_update_index(const struct UpdateSet *update_rules,const unsigned long update) {
    unsigned long update_index;
    for(update_index = 0; update_index < update_rules->updates; update_index++) {
        if(update_rules->update[update_index] == update) return update_index;
    }
    return ULONG_MAX;
}

struct Rule find_invalid_rule(const struct RuleSet *page_rules, const struct RuleSet *temp_rules) {
    unsigned long page_index, temp_index;
    struct Rule page_rule, temp_rule;

    if(page_rules->count >= MAX_RULES || temp_rules->count >= MAX_RULES) { printf("Error: RuleSet count variable corrupted.\n"); exit(EXIT_FAILURE); }

    for(temp_index = 0; temp_index < temp_rules->count; temp_index++) {
        temp_rule = temp_rules->rules[temp_index];
        for(page_index = 0; page_index < page_rules->count; page_index++) {
            page_rule = page_rules->rules[page_index];
            if(temp_rule.precedes == page_rule.follows && temp_rule.follows == page_rule.precedes)
            {
                return temp_rule;
            }
        }
    }
    struct Rule no_invalid = {0};
    return no_invalid;
}

unsigned long calc_result(const struct UpdateList *update_list, const struct RuleSet *page_rules) {
    unsigned long update_index;
    unsigned long result = 0;

    for(update_index = 0; update_index < update_list->sets; update_index++) {
        struct RuleSet temp_rules = {0};
        struct UpdateSet current_update = update_list->update_set[update_index];

        unsigned long middle_index = current_update.updates/2;
        construct_temp_ruleset(&current_update,&temp_rules);
        if(valid_update_set(page_rules, &temp_rules) == TRUE) {
            result += current_update.update[middle_index];
        }
    }
    return result;
}

void construct_temp_ruleset(const struct UpdateSet *update_set, struct RuleSet *temp_rules) {
    unsigned long update_index;

    temp_rules->count = 0;

    if(update_set->updates >= MAX_UPDATES) { printf("Error UpdateSet updates variable corrupted.\n"); exit(EXIT_FAILURE); }

    for(update_index = 0; update_index < update_set->updates; update_index++) {
        unsigned long temp_index;
        for(temp_index = update_index + 1; temp_index < update_set->updates; temp_index++) {
            if(temp_rules->count >= MAX_RULES) { printf("Error: Rules buffer not big enough to hold Temp Rules\n"); exit(EXIT_FAILURE); }
            struct Rule temp_rule = {update_set->update[update_index],update_set->update[temp_index]};
            temp_rules->rules[temp_rules->count] = temp_rule;
            temp_rules->count += 1;
        }
    }
}

bool valid_update_set(const struct RuleSet *page_rules, const struct RuleSet *temp_rules) {
    bool valid = TRUE;
    unsigned long page_index, temp_index;
    struct Rule temp_rule, page_rule;

    if(page_rules->count >= MAX_RULES || temp_rules->count >= MAX_RULES) { printf("Error: RuleSet count variable corrupted.\n"); exit(EXIT_FAILURE); }

    for(temp_index = 0; temp_index < temp_rules->count && valid; temp_index++) {
        temp_rule = temp_rules->rules[temp_index];
        for(page_index = 0; page_index < page_rules->count && valid; page_index++) {
            page_rule = page_rules->rules[page_index];
            if(temp_rule.follows == page_rule.precedes &&
               temp_rule.precedes == page_rule.follows)
                    valid = FALSE;
        }
    }
    return valid;
}

void print_input(const struct RuleSet *page_rules, const struct UpdateList *update_list) {
    unsigned long rule;

    for(rule = 0 ; rule < page_rules->count; rule++) {
        printf("%lu|%lu\n",page_rules->rules[rule].precedes,page_rules->rules[rule].follows);
    }

    unsigned long set,update;

    for(set = 0; set < update_list->sets; set++) {
        printf("[");
        for(update = 0; update < update_list->update_set[set].updates; update++) {
            printf("%lu", update_list->update_set[set].update[update]);
            if(update < update_list->update_set[set].updates - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

void input_handler(const char* filename, struct RuleSet *page_rules, struct UpdateList *update_list) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file.");
        exit(EXIT_FAILURE);
    }

    page_rules->count = 0;

    char line[LINE_BUFFER];
    char *token, *endptr;
    bool updating = FALSE;

    while (fgets(line, sizeof(line), file)) {

        if(line[0] == '\n') {
            updating = TRUE;
            update_list->sets = 0;
            continue;
        }

        if(!updating) {

            char *precedes = strtok(line, "|");
            char *follows  = strtok(NULL, "|");
            if (strtok(NULL, "|") != NULL) {
                printf("Error: Invalid input format. Too many '|' on rule %s", line);
                continue;
            }

            if(precedes != NULL && follows != NULL) {

                char *end_pre, *end_fol;

                errno = 0;
                unsigned long precedes_val = strtoul(precedes, &end_pre, 10);
                if (errno == ERANGE) {
                    printf("Error: Number out of range for precedes on rule %s", line);
                    continue;
                }

                errno = 0;
                unsigned long follows_val = strtoul(follows, &end_fol, 10);
                if (errno == ERANGE) {
                    printf("Error: Number out of range for follow on rule %s", line);
                    continue;
                }

                if(page_rules->count < MAX_RULES) {
                    if(*end_pre != '\0' || *end_fol != '\n') {
                        printf("Invalid Number in line: %s\n", line);
                        continue;
                    }

                    page_rules->rules[page_rules->count].precedes = precedes_val;
                    page_rules->rules[page_rules->count].follows  = follows_val;
                    page_rules->count++;
                }

                else {
                    printf("Too many inputs. Buffer size too small.");
                    exit(EXIT_FAILURE);
                }
            }
        }

        if(updating) {

            if(update_list->sets >= MAX_UPDATE_SETS){
                printf("Error: too many update sets.");
                exit(EXIT_FAILURE);
            }

            unsigned long updates_index = 0;
            token = strtok(line, ",");

            while (token != NULL) {
                if(updates_index >= MAX_UPDATES) {
                    printf("Error: Too many updates for this set.");
                    exit(EXIT_FAILURE);
                }
                errno = 0;
                unsigned long update = strtoul(token,&endptr,10);
                if(errno == ERANGE) {
                    printf("Error: Number out of range for update on update %s", line);
                    continue;
                }

                update_list->update_set[update_list->sets].update[updates_index] = update;

                if((*endptr != '\0' && *endptr != '\n')) {
                    printf("Error: Invalid Number in line: %s\n", line);
                    exit(EXIT_FAILURE);
                }

                updates_index++;
                token = strtok(NULL, ",");
            }
            update_list->update_set[update_list->sets].updates = updates_index;
            update_list->sets++;
        }
    }
    fclose(file);
}
