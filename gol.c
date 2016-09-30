"""
Author: Uriel Mandujano
C Implementation of Game of Life
"""

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "gol.h"

int main(int argc, char **argv) {
    validate_input(argc, argv);
    validate_input_file(argv[1]);

    Board board;
    set_up_board(argv[1], &board);
    for (int i = 0; i < board.iterations; i++) {
        show_board(&board);
        update_board(&board);
        sleep(1);
    }
    free_board(&board);
}

void update_board(Board* b) {
    Point dead[b->rows * b->cols];
    int curr_death = 0;
    Point alive[b->rows * b->cols];
    int curr_living = 0;

    for (int i = 0; i < b->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int live_neighbors = calculate_live_neighbors(b, i, j);
            Point curr_point = make_point(i, j);

            if (b->board[i][j] == 'X') {
                if (live_neighbors < 2 || live_neighbors > 3) {
                    dead[curr_death++] = curr_point;
                }
                else {
                    alive[curr_living++] = curr_point;
                }
            }
            else {
                if (live_neighbors == 3) {
                    alive[curr_living++] = curr_point;
                }
            }
        }
    }
    update_cells(b, dead, curr_death, alive, curr_living);
}

Point make_point(int i, int j) {
    Point curr_point = {
        .row = i,
        .col = j
    };
    return curr_point;
}

void update_cells(Board* b, Point* dead, int num_dead, \
                  Point* alive, int num_alive) {
    update_cell_content(b, dead, num_dead, '-');
    update_cell_content(b, alive, num_alive, 'X');
}

void update_cell_content(Board* b, Point* points, int num_p, char status) {
    for (int i = 0; i < num_p; i++) {
        Point currp = points[i];
        int currp_x = currp.row;
        int currp_y = currp.col;
        b->board[currp_x][currp_y] = status;
    }
}

int calculate_live_neighbors(Board* b, int i, int j) {
    int live_neighbors = 0;
    int max_i = b->rows;
    int max_j = b->cols;
    char** board = b->board;

    if (board[safe_wrap(i + 1, max_i)][j] == 'X') {
        ++live_neighbors;
    }
    if (board[safe_wrap(i - 1, max_i)][j] == 'X') {
        ++live_neighbors;
    }
    if (board[i][safe_wrap(j + 1, max_j)] == 'X') {
        ++live_neighbors;
    }
    if (board[i][safe_wrap(j - 1, max_j)] == 'X') {
        ++live_neighbors;
    }
    if (board[safe_wrap(i + 1, max_i)][safe_wrap(j + 1, max_j)] == 'X') {
        ++live_neighbors;
    }
    if (board[safe_wrap(i + 1, max_i)][safe_wrap(j - 1, max_j)] == 'X') {
        ++live_neighbors;
    }
    if (board[safe_wrap(i - 1, max_i)][safe_wrap(j + 1, max_j)] == 'X') {
        ++live_neighbors;
    }
    if (board[safe_wrap(i - 1, max_i)][safe_wrap(j - 1, max_j)] == 'X') {
        ++live_neighbors;
    }
    return live_neighbors;
}

int safe_wrap(int i, int max_i) {
    if (i < 0) {
        return max_i - 1;
    }
    if (i >= max_i) {
        return 0;
    }
    return i;
}

void clear_previous_board() {
    system("clear");
}

void set_up_board(char* filename, Board* b) {
    int curr_line = 0;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    FILE* f = fopen(filename, "r");
    while ((read = getline(&line, &len, f)) != -1) {
        line[read-1] = '\0';
        if (curr_line < 3) {
            int num = sscanf(line, "%d", &num);
            if (curr_line == 0) {
                b->rows = num;
            }
            if (curr_line == 1) {
                b->cols = num;
            }
            if (curr_line == 2) {
                b->iterations = num;
            }
        }
        if (curr_line == 3) {
            make_board(b);
        }
        if (curr_line > 3) {
            set_board_cells(b, line);
        }
        ++curr_line;
    }
    free(line);
    fclose(f);
    return;
}

void set_board_cells(Board* b, char* line) {
    char* tok = strtok(line, " ");
    int i = 0;
    int coords[2];
    while (tok != NULL) {
        sscanf(tok, "%d", &coords[i]);
        tok = strtok (NULL, " ");
        ++i;
    }
    b->board[coords[0]][coords[1]] = 'X';
}

void make_board(Board* b) {
    b->board = (char **) malloc(b->rows * sizeof(char *));
    for (int i = 0; i < b->rows; i++) {
        b->board[i] = (char*) malloc(b->cols * sizeof(char));
    }

    for(int i = 0; i < b->rows; ++i) {
        for(int j = 0; j < b->cols ; ++j) {
            b->board[i][j] = '-';
        }
    }
}

void show_board(Board* b) {
    for(int i = 0; i < b->rows; ++i) {
        for(int j = 0; j < b->cols ; ++j) {
            printf("%c", b->board[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void free_board(Board* b) {
    for (int i = 0; i < b->rows; i++){
        free(b->board[i]);
    }
    free(b->board);
}

void validate_input(int argc, char** argv) {
    if (argc != 2) {
        usage();
    }
    if (access(argv[1], F_OK) == -1) {
        usage();
    }
}

void validate_input_file(char* filename) {
    printf("Checking %s for valid format... ", filename);
    size_t len = 0;
    ssize_t read;
    int curr_line = 0;
    char* line = NULL;
    int expected_lines = 4;

    FILE* f = fopen(filename, "r");
    while ((read = getline(&line, &len, f)) != -1) {
        // remove trailing newlines
        line[read-1] = '\0';

        if (curr_line < 3) {
            for (int i = 0; i < read-1; i++) {
                if ((isdigit(line[i]) == 0)) {
                    printf("Failure - File is incorrect format\n");
                    file_format();
                }
            }
        }
        if (curr_line == 3) {
            expected_lines = expected_lines + strtol(line, NULL, 10);
        }
        // Remaining lines are valid coordinates
        if (curr_line > 3) {
            char* tok = strtok(line, " ");
            while (tok != NULL) {
                if (isdigit(*tok) == 0) {
                    printf("Failure - File coordinates non numerical\n");
                    file_format();
                }
                tok = strtok (NULL, " ");
            }
        }
        ++curr_line;
    }
    free(line);
    fclose(f);

    if (curr_line != expected_lines) {
        printf("Failure - File is of incorrect length\n");
        file_format();
    }
    printf("Success\n");
}

void usage() {
    printf("Invalid program call\n");
    printf("gol.c filename\n");
    exit(EXIT_FAILURE);
}

void file_format() {
    printf("Proper file format:\n");
    printf("Num rows\n");
    printf("Num cols\n");
    printf("Num iterations\n");
    printf("Num following coordinate pairs\n");
    printf("coordinate1x coordinate1y\n");
    exit(EXIT_FAILURE);
}
