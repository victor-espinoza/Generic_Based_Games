/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: TicTacToeBoard.cpp
* 
* Description: CPP file for the TicTacToeBoard class, which is a child class
* of Gameboard. Functions that are not implemented in the GameBoard class are
* implemented in this file.
*
*/

#include "TicTacToeBoard.h"


/* Default constructor initializes the board to its starting "new game" 
state */
TicTacToeBoard::TicTacToeBoard() {
   //initialize the whole board to 0
   for (int i = 0; i < TTT_BOARD_SIZE; i++)
      mTTTBoard[i] = 0;
   mValue = 0, mPassCount = 0;
   mNextPlayer = TTT_P2;
}//close OthelloBoard()	 default constructor


/* Fills in a vector with all possible moves on the game board for the
current player. If no moves are possible, then a single TicTacToeMove
representing a PASS is put in the vector. Any code that calls ApplyMove 
is responsible for first checking that the requested move is among the 
possible moves reported by this function.
Precondition: vector is empty.
Postcondition: vector contains all valid moves for the current player. */ 

void TicTacToeBoard::GetPossibleMoves(vector<GameMove*>* list) const {
   //Local Variables:
   //keeps track of the current player
   char currentPlayer = (mNextPlayer == TTT_P2 ? TTT_P1 : TTT_P2);
   //this variable is to determine whether the user must pass or not
   bool  mustPass = true;
   /* Look through every spot on the board to see if the user can move 
   to the designated spot on the gameboard. */
   for (char i = 0; i < TTT_BOARD_SIZE; i++) {
      /* Move to the space occupied by(i).Make sure that the space is
      InBounds and is empty, otherwise this would not be a valid move. */
      if (InBounds(i) && mTTTBoard[i] == 0) {
         /* If the current location is inboundsand is empty, set the
         mustPass variable to false. */
         mustPass = false;
         /* This means that this space is a valid possible move. Push
         the move onto the back of the list. */
         TicTacToeMove* possibleMove = (TicTacToeMove*)CreateMove();
         *possibleMove = TicTacToeMove(i);
         list->push_back(possibleMove);
      }//end if
   }//end for loop
   /* If the mustPass variable is still true, then the user must pass. 
   We push the pass move into the back of the list. */
   if (mustPass) {
      TicTacToeMove* passMove = (TicTacToeMove*)CreateMove();
      *passMove = TicTacToeMove();
      list->push_back(passMove);
   }//end if 
}//close GetPossibleMoves(...) 


/* Applies a valid move to the board, updating the board state
accordingly. You may assume that this move is valid, and is consistent
with the list of possible moves returned by GetPossibleMoves. */
void TicTacToeBoard::ApplyMove(GameMove* move) {
   //Local Variables:
   TicTacToeMove* m = (TicTacToeMove*)move;
   //keeps track of the current player
   char currentPlayer = (mNextPlayer == TTT_P2 ? TTT_P1 : TTT_P2);
   //make sure move is inbounds and the spot is empty on the game board
   if (InBounds(m->mIndex) && mTTTBoard[m->mIndex] == 0) {
      mTTTBoard[m->mIndex] = currentPlayer; //update board 
      mValue = UpdateBoardValue(); //update board value
      mHistory.push_back(m); //add move to back of history vector
   }//end if	
   //if the move is a PASS, we add a PASS move to the history vector
   if (m->IsPass()) {
      mHistory.push_back(m);
   }//end if 
   /* Increment the pass count if the move was a pass or set it to 0 
   otherwise */
   mPassCount = (m->IsPass() ? mPassCount + 1 : 0);
   //change the value of mNextPlayer accordingly
   mNextPlayer = (mNextPlayer == TTT_P2 ? TTT_P1 : TTT_P2);
}//close ApplyMove(...)


/* Undoes the last move applied to the board, restoring it to the state
it was in prior to the most recent move (including whose turn it is, 
what the board value is, and what the pass count is). */
void TicTacToeBoard::UndoLastMove() {
   //only undo a move if there is an actual move in the history vector
   if(!mHistory.empty()){
      //set the currentPlayer variable with the appropriate value
      char currentPlayer = (mNextPlayer == TTT_P2 ? TTT_P1 : TTT_P2);
      //set the deleteMove variable to the most recent move
      TicTacToeMove* deleteMove = (TicTacToeMove*)mHistory.back();

      //only update board while deleting moves that are not passes
      bool moveIsAPass = deleteMove->IsPass();
      if (!moveIsAPass) {
         //change the piece back to unoccupied
         mTTTBoard[deleteMove->mIndex]= 0;
      }//end if

      /* Delete the move after you are done undoing itand pop it from 
      the back of the mHistory vector */
      delete deleteMove;
      mHistory.pop_back();

      //update mNextPlayer and mPassCount accordingly
      mNextPlayer = (mNextPlayer == TTT_P2 ? TTT_P1 : TTT_P2);
      mPassCount = ((GetMoveCount() > 0) ? 
       ((mPassCount == 2 && (moveIsAPass)) ? 1: 0) : 0);
      //update board value because the game board has changed.
      mValue = UpdateBoardValue(); 
   }//end if
}//close UndoLastMove()


/* Undoes any active move on the Othello board and resets it to it's
initial un-played state */
void TicTacToeBoard::ResetGameBoard() {
   int appliedMoveCount = GetMoveCount();
   //undo all applied moves and reset game board values to 0
   if (appliedMoveCount > 0) {
      for (int i = 0; i < appliedMoveCount; i++)
         UndoLastMove();
   }//end if
   //Finally, reset board value, pass count, and the next player
   mValue = 0, mPassCount = 0;
   mNextPlayer = TTT_P2;
}//close ResetGameBoard()


/* Checks to see if the game is finished yet and updates the game 
board's value accordingly. */
int TicTacToeBoard::UpdateBoardValue() {
   /* Array contains all possible winning line combinations (each line 
   is made up of 3 points) */
   int winCombos[] = { 0, 1, 2,   3, 4, 5,   6, 7 ,8,   0, 3, 6,   
    1, 4, 7,   2, 5, 8,   0, 4, 8,   2, 4, 6 };
   //Determine the winner
   for (int i = 0; i < ARRAYSIZE(winCombos); i += 3) {
      //see if there are any winning lines
      if (mTTTBoard[winCombos[i]] != 0 && 
       mTTTBoard[winCombos[i]] == mTTTBoard[winCombos[i + 1]] && 
       mTTTBoard[winCombos[i]] == mTTTBoard[winCombos[i + 2]]) 
         //return the appropriate winner
         return mTTTBoard[winCombos[i]] == TTT_P1 ? 3 : -3;
   }//end for
   /* If we reach the end of the loop, then that means that nobody 
   has won yet. We return a value of 0 to account for this. */
   return 0;
}//close UpdateBoardValue()


/* Populates a given vector with the indexes of the winning player's
game pieces. Only called when the game is finished. */
void TicTacToeBoard::UpdateWinningCells(vector<int>* list) {
   /* Array contains all possible winning line combinations (each line 
   is made up of 3 points) */
   if (IsFinished()) {
      int winCombos[] = { 0, 1, 2,   3, 4, 5,   6, 7 ,8,   0, 3, 6,  
       1, 4, 7,   2, 5, 8,   0, 4, 8,   2, 4, 6 };
      bool someoneWon = FALSE;
      //Determine the winner
      for (int i = 0; i < ARRAYSIZE(winCombos); i += 3) {
         if (mTTTBoard[winCombos[i]] != 0 && 
          mTTTBoard[winCombos[i]] == mTTTBoard[winCombos[i + 1]] &&
          mTTTBoard[winCombos[i]] == mTTTBoard[winCombos[i + 2]]) {
            /* Assign the winning line indexes to the wins array so we 
            can display the winning line*/
            someoneWon = TRUE;
            list->push_back(winCombos[i]);
            list->push_back(winCombos[i + 1]);
            list->push_back(winCombos[i + 2]);
            break; //we found a winner, so break from the loop
         }//end if
      }//end for

      //It's a tie, so add all occupied cell indexes to the given list
      if (!someoneWon) {
         for (int i = 0; i < TTT_BOARD_SIZE; i++) {
            if (mTTTBoard[i] != 0)
               list->push_back(i);
         }//end for
      }//end if
   }//end if
}//close UpdateWinningCells(...)


/* Returns the value of the game board at a specified index */
char TicTacToeBoard::GetBoardValueAtIndex(int index) {
   return (index >= 0 && index < TTT_BOARD_SIZE) ? 
    mTTTBoard[index] : 0;
}//close GetBoardValueAtIndex(...)


/* Returns the current player score given a specified player. For
Tic-Tac-Toe, the player's score only changes when they have won the
game. */
int TicTacToeBoard::GetPlayerScore(char player) const {
   /* Player score is irrelevant for Tic-Tac-Toe (either a player 
   wins, or both players have the same score) so we just return 0 */
   return 0;
}//close GetPlayerScore(...)


/* Returns TRUE if the game is finished (If either player has 
successfully created a line with their game pieces. */
bool TicTacToeBoard::IsFinished() const {
   bool boardFull = TRUE;
   //iterate through game board to see if it is full
   for (int i = 0; i < TTT_BOARD_SIZE; i++) {
      //if it isn't full, then set the variable to false
      if (mTTTBoard[i] == 0) {
         boardFull = FALSE;
         break; 
      }//end if
   }//end for
   return boardFull || mPassCount == 2 || mValue == 3 || 
    mValue == -3;
}//close IsFinished()