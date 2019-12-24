/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init();
void draw();
bool move(int tile);
bool won(void);
void search_and_assign(int tile);
int tile_row_index;
int tile_col_index;

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

// Clears screen using ANSI escape sequences
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

// Greets player.
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

// Initializes the game's board with tiles numbered 1 through d*d - 1 (i.e., fills 2D array with values but does not actually print them)
void init()
{
    // print board decsending from d*d-1
    int k = 0;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = d*d - 1 - k;
            k++;
        }
    }

    // if d is even, swap 1 and 2
    if ( d % 2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

// Prints the board in its current state.
void draw()
{
    for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                if (board[i][j] == 0)
                {
                    printf("_  ");
                }
                else if ( board[i][j] < 10)
                {
                    printf("%i  ", board[i][j]);
                }
                else
                {
                    printf("%i ", board[i][j]);
                }
            }
            printf("\n");
        }
}

// If tile borders empty space, moves tile and returns true, else returns false.
bool move(int tile)
{
    if ( tile > d*d - 1 || tile < 1 )
    {
        return false;
    }

    // obtain row and column index of tile
    search_and_assign(tile);

    // check below
    if ( tile_row_index > 0 && board[tile_row_index - 1][tile_col_index] == 0 )
    {
       board[tile_row_index - 1][tile_col_index] = tile;
       board[tile_row_index][tile_col_index] = 0;
       return true;
    }
    // check above
    else if ( tile_row_index < d - 1 && board[tile_row_index + 1][tile_col_index] == 0 )
    {
       board[tile_row_index + 1][tile_col_index] = tile;
       board[tile_row_index][tile_col_index] = 0;
       return true;
    }
    // check to the left
    else if ( tile_col_index > 0 && board[tile_row_index][tile_col_index - 1] == 0 )
    {
       board[tile_row_index][tile_col_index - 1] = tile;
       board[tile_row_index][tile_col_index] = 0;
       return true;
    }
    // check to the right
    else if ( tile_col_index < d - 1 && board[tile_row_index][tile_col_index + 1] == 0 )
    {
       board[tile_row_index][tile_col_index + 1] = tile;
       board[tile_row_index][tile_col_index] = 0;
       return true;
    }
    return false;
}

// returns true if game is won (i.e., board is in winning configuration), else false
bool won(void)
{
    int c = 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (  board[i][j] == c )
            {
                c++;
            }
        }
    }

    if ( board[0][0] == 1 && board[d-1][d-1] == 0 && c == d*d )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// searches for tile in the board and assigns its row and column indicies to memory
void search_and_assign(int tile)
{
   for (int i = 0; i < d; i++)
   {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                tile_row_index = i;
                tile_col_index = j;
                return;
            }
        }
   }
}