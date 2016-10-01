typedef struct
{
    int rows;
    int cols;
    int iterations;
    char** board;
} Board;

typedef struct
{
    int row;
    int col;
} Point;

typedef struct
{
    Board* b;
    int max_i;
    int min_i;
} Range;

/* Processes input to determine if it is valid */
void validate_input(int argc, char** argv);

/* Checks input file format to be correct */
void validate_input_file(char* filename);

/* Converts given coordinates into a Point struct */
Point make_point(int i, int j);

/* Updates the status of a cell given a neighbor count */
void update_cells(Board* board, Point* dead, int curr_dead, \
                  Point* alive, int curr_alive);

/* Helper function to update_cells */
void update_cell_content(Board* b, Point* points, int num_p, char status);

/* Counts the number of live neighbors surrounding a cell */
int calculate_live_neighbors(Board* board, int i, int j);

/* Threaded function to iterate through specified rows */
void* check_rows(void* args);

/* Clears the life/death arrays and their counts */
void reset_lifes_deaths();

/* Prepares the Range struct with proper values for use in thread */
Range* prepare_thread_args(Board* b, int max_thds, int curr_thd, int range);

/* Updates the board positions using nthids threads */
void update_board(Board* board, int nthids);

/* Reads input file and sets up board */
void set_up_board(char* filename, Board* board);

/* Places 'alive' character in specific places on board */
void set_board_cells(Board* b, char* line);

/* Makes a system call to clear terminal contents */
void clear_previous_board();

/* Performs the given operation taking care to not go over max_i or
   below 0 */
int safe_wrap(int i, int max_i);

/* Mallocs up memory for the game board */
void make_board(Board* board);

/* Prints the current board contents to the user */
void show_board(Board* b);

/* Frees board memory */
void free_board(Board* b);

/* Writes to stdout proper usage of file and exits */
void usage();

/* Writes to stdout proper file format */
void file_format();

/* Converts a char array from the command line into an int */
int arg_to_int(char* str);
