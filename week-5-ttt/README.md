# Tic Tac Toe Minimax Overview

* The game board is represented as a NxN dimensional array in row-major form.
* the `win` function looks at a board, and returns `1` if the board is a winning position for the current player, and -1 otherwise.
* the `playerMove()` function gets user input.  
* the `computerMove` function picks the best move for the computer.  At the heart of this is the `for i` loop that iterates through every board position (`board[i]`), checks to see if the board is empty, and if it is, places the player's marker in that board position and then calls minimax on the new board (swapping players).  After the loop, it picks the best scoring move among all possible, and makes that move by changing the board value.
* the `minimax` function is a recursive implementation of the minimax algorithm (technically this is negamax, I believe).  
    * The `for i` loop iterates through every board position (`board[i]`), check to see if the board is empty there, and if it is, places the player's marker in that board position and then (recursively) calls minimax on the new board (swapping players).  Like all good recursive functions you can trust that when minimax returns it'll return the highest scoring move *for the other player*.  


## Tips for Parallelizing

* you only want the root node asking the user for input every move.
* the obvious thing to parallelize is the `computerMove` function.  And the fastest way to do *this* is to simply do cyclic allocation of values of `i` for the `for i` loop. (you don't want to try parallelizing recursion - yikes!).
* you'll need a way to broadcast the current state of the board to all nodes prior to the for loop.
* you'll need a clever way to find which node has the best move (i.e. a `Reduce` operation)
* only the root node should make the best move.
* you'll need to modify `main` so that only the root node draws the board and asks the player for their move. 
* challenge: scale up to bigger boards.