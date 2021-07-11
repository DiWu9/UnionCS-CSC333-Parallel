#include <stdio.h>
#include <mpi.h>
//to compile: mpicc -std=c99 -Wall MPI_ttt.c -o MPI_ttt
//to run (simple): mpirun -np (#tasks) ./MPI_ttt

#define BOARDSIZE 9

char gridChar(int i)
{
    switch (i)
    {
    case -1: // player
        return 'X';
    case 0: // empty
        return ' ';
    case 1: // computer
        return 'O';
    }
    return ' ';
}

void draw(int *b)
{
    //print the board
    printf(" %c | %c | %c\n", gridChar(b[0]), gridChar(b[1]), gridChar(b[2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", gridChar(b[3]), gridChar(b[4]), gridChar(b[5]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", gridChar(b[6]), gridChar(b[7]), gridChar(b[8]));
}

int win(int *board)
{
    //determines if a player has won, returns 0 otherwise.
    //kind of inelegant and non-scalable to hard-code wins
    unsigned int wins[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};
    int i;
    for (i = 0; i < 8; ++i)
    {
        //for the given current winning board
        // does our board have matching pieces at each of those locations?
        unsigned int curwin0 = wins[i][0];
        unsigned int curwin1 = wins[i][1];
        unsigned int curwin2 = wins[i][2];
        //if any three pos in wins are occupied by the same player, he/she/it wins
        if (board[curwin0] != 0 &&
            board[curwin0] == board[curwin1] &&
            board[curwin0] == board[curwin2])
            return board[curwin2]; //if yes, return the player (-1 or 1) who wins
    }
    return 0;
}

//minimax in a single recursive function
// you call max if it is your move
// and min if it is your opponent's move.
int minimax(int *board, int player)
{
    //How is the position like for player (their turn) on board?
    int winner = win(board); //is the board a win?
    if (winner != 0)
        return winner * player; //base case

    int curbestmove = -1;  //the best move possible
    int curbestscore = -2; //Losing moves are preferred to no move
    int i;
    for (i = 0; i < BOARDSIZE; ++i)
    { //For all moves,
        if (board[i] == 0)
        {                      //If legal,
            board[i] = player; //Try the move
            int thisScore = -1 * minimax(board, player * -1);
            if (thisScore > curbestscore)
            {
                curbestscore = thisScore;
                curbestmove = i;
            }             //Pick the one that's worst for the opponent
            board[i] = 0; //Reset board after try
        }
    }
    if (curbestmove == -1)
        return 0;
    return curbestscore;
}

void computerMove(int *board, int my_rank, int comm_sz)
{
    int move = -1;
    int score = -2;
    int i;
    //333 Project: This loop is the easiest part to parallelize
    for (i = my_rank; i < BOARDSIZE; i += comm_sz)
    {
        if (board[i] == 0)
        {
            board[i] = 1;
            int tempScore = -minimax(board, -1);
            board[i] = 0;
            if (tempScore > score)
            {
                score = tempScore;
                move = i; // best move for each task
            }
        }
    }
    int localscoremove[2] = {score, move};
    int globalscoremove[2];
    MPI_Reduce(localscoremove, globalscoremove, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD); // get the best score and its move
    if (my_rank == 0)
    {
        board[globalscoremove[1]] = 1; // root node makes the best move
    }
}

void playerMove(int board[9])
{
    int move = 0;
    do
    {
        printf("Input move ([0..8]): \n");
        fflush(stdout);
        scanf("%d", &move);
        printf("\n");
    } while (move >= BOARDSIZE || move < 0 || board[move] != 0); // try another move if current is invalid
    board[move] = -1;
}

int main(int argc, char *argv[])
{

    int comm_sz; /* Number of processes    */
    int my_rank; /* My process rank        */
    //Don't put any MPI stuff before this line
    MPI_Init(&argc, &argv); //sets up the MPI. Always this line!

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int board[BOARDSIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int player = 0;
    double start;
    double end;

    if (my_rank == 0)
    {
        printf("Computer: O, You: X\nPlay (1)st or (2)nd? ");
        fflush(stdout);
        scanf("%d", &player);
        printf("\n");
    }

    for (unsigned turn = 0; turn < BOARDSIZE && win(board) == 0; ++turn)
    {
        if ((turn + player) % 2 == 0)
        {
            if (my_rank == 0)
                start = MPI_Wtime();
            MPI_Bcast(board, BOARDSIZE, MPI_INT, 0, MPI_COMM_WORLD); // broadcast the current board
            computerMove(board, my_rank, comm_sz);
            if (my_rank == 0)
            {
                end = MPI_Wtime();
                printf("The root task took %.2f ms to find the move.\n", (end - start) * 1000);
            }
        }
        else
        {
            if (my_rank == 0)
            {
                draw(board);
                playerMove(board);
            }
        }
    }

    if (my_rank == 0)
    {
        switch (win(board))
        {
        case 0:
            printf("A draw. How droll.\n");
            break;
        case 1:
            printf("You lose.\n");
            break;
        case -1:
            printf("You win. Inconceivable!\n");
            break;
        }
        draw(board);
    }

    MPI_Finalize();
    //please no MPI stuff after this line
    return 0;
}