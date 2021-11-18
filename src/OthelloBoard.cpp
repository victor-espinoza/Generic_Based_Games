/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: OthelloBoard.cpp
*
* Description: CPP file for the OthelloBoard class, which is a child class
* of Gameboard. Functions that are not implemented in the GameBoard class are
* implemented in this file.
*
*/

#include "OthelloBoard.h"

/* Default constructor initializes the board to its starting "new game" state */
OthelloBoard::OthelloBoard() {
   //initialize the whole board to 0
   for (int i = 0; i < OTHELLO_BOARD_SIZE; i++) 
      mOthelloBoard[i]= 0;

   //set the 4 middle pieces to their appropriate values
   mOthelloBoard[P2_INIT_IND1] = -1;
   mOthelloBoard[P1_INIT_IND1] = 1;
   mOthelloBoard[P1_INIT_IND2] = 1;
   mOthelloBoard[P2_INIT_IND2] = -1;

   //Initialize the Value, PassCount and NextPlayer of the board
   mValue = 0, mPassCount = 0;
   mNextPlayer = OTHELLO_P2;
}//close OthelloBoard()


/* Fills in a vector with all possible moves on the game board for the
current player. If no moves are possible, then a single OthelloMove
representing a PASS is put in the vector. Any code that calls ApplyMove 
is responsible for first checking that the requested move is among the 
possible moves reported by this function.
Precondition: the vector is empty.
Postcondition: the vector contains all valid moves for the current player. */
void OthelloBoard::GetPossibleMoves(vector<GameMove*>* list) const {
   //Local Variables:
   //keeps track of the current player
   char currentPlayer = (mNextPlayer == OTHELLO_P2 ? OTHELLO_P1 : OTHELLO_P2);
   bool validMove = false; //keeps track of the validity of a move
   bool mustPass = true; //keeps track of if the user must pass
   int offsetAmount = 0; //keeps track of the offset amount that is being used
   int runningIndex = 0; //keeps track of the running index on the game board

   //iterate through every index on the game board
   for (int ind = 0; ind < OTHELLO_BOARD_SIZE; ind++) {
      /*Look through all 8 directions on every spot on the board to see if you 
      can caputure your opponent's pieces. */
      for (int dir = 0; dir < DIRECTIONS; dir++) {
         validMove = false; //set validMove to false by default.

         //Determine the offset amount for the direction that is being taken
         offsetAmount = GetDirectionalOffset(ind, dir);

         /* Move one space in the designated direction. (0 = NW, 1 = N, 2 = NE, 
         3 = E, 4 = SE, 5 = S, 6=SW, 7=W) Make sure that the space is InBounds 
         (relative to the direction) and that it is not empty. Also, make sure 
         that the original location was empty, otherwise this would not be a 
         valid move. */
         if (DirectionalInBounds(ind, ind + offsetAmount, dir) &&
          (mOthelloBoard[ind + offsetAmount] != 0) && mOthelloBoard[ind] == 0) {
            //If the space is the enemy, then you update the running index.
            if (mOthelloBoard[ind + offsetAmount] == mNextPlayer) {
               runningIndex = ind + offsetAmount;
               /* While you are InBounds of the board, the current spot is an 
               enemy piece, and you originally detected an enemy piece, you go 
               one more space in the same direction. Repeat until the gameBoard 
               location is no longer the enemy. */
               while (DirectionalInBounds(runningIndex - offsetAmount, 
                runningIndex, dir) && mOthelloBoard[runningIndex] == 
                mNextPlayer) {
                  runningIndex += offsetAmount; //update runningIndex
               }//end while loop

               /* If the current location is inbounds and is your own piece, 
               set the validMove variable to true and the mustPass variable  
               to false and then exit the inner loop. */
               if (DirectionalInBounds(runningIndex - offsetAmount, 
                runningIndex, dir) && mOthelloBoard[runningIndex] == 
                currentPlayer) {
                  validMove = true; //update the validMove variable
                  mustPass = false; //update the mustPass variable
                  break;
               }//end if
            }//end if
         }//end if
      }//end inner for loop

      //if it was a valid move, then push it onto the back of the list
      if (validMove) {
         OthelloMove* possibleMove = (OthelloMove*)CreateMove(ind);
         list->push_back(possibleMove);
      }//end if
   }//end outer for loop

   /* If the mustPass variable is still true, then the user must pass. I push
   the pass move into the back of the list. */
   if (mustPass) {
      OthelloMove* passMove = (OthelloMove*)CreateMove();
      *passMove = OthelloMove();
      list->push_back(passMove);
   }//end if
}//close GetPossibleMoves(...) 


/* Applies a valid move to the board, updating the board state accordingly. 
You may assume that this move is valid, and is consistent with the list of 
possible moves returned by GetPossibleMoves. */
void OthelloBoard::ApplyMove(GameMove* move) {
   //Local Variables:
   OthelloMove* m = (OthelloMove*)move; //Cast the game move to an OthelloMove
   //keeps track of the current player
   char currentPlayer = (mNextPlayer == OTHELLO_P2 ? OTHELLO_P1 : OTHELLO_P2);
   bool validMove = false; //keeps track of validity of move
   int runningIndex = 0; //keeps track of the running index of the board
   int dirOffset = 0; //keeps track of the directional offset value

   //keeps track of the number of pieces that were switched per flipset
   char switchedPieces = 0;
   
   /* If the move was a pass, then we do not need to execute any of the 
   following code. */
   if (!m->IsPass()) {
      /* Look through all 8 directions to see if you can caputure your 
      opponents pieces. */
      for (int dir = 0; dir < DIRECTIONS; dir++) {
         /* If the move was a pass, then we do not need to execute any of the
         following logic. Exit the loop immediately! */
         if (m->IsPass())
            break;

         switchedPieces = 0; //reset the number of switched pieces
         dirOffset = GetDirectionalOffset(m->mIndex, dir); //get offset value

         if (DirectionalInBounds(m->mIndex, m->mIndex + dirOffset, dir)
          && (mOthelloBoard[m->mIndex + dirOffset] != 0)) {
            /* If the space is the enemy, then you set the runningIndex to 
            keep track of where you are on the gameboard. */
            if (mOthelloBoard[m->mIndex + dirOffset] == mNextPlayer)
               runningIndex = m->mIndex + dirOffset; //initialize runningIndex

            /* While you are InBounds of the board and the current spot is an
            enemy piece, you go one more space in the same direction. Repeat
            until the gameBoard location is no longer the enemy. */
            while (DirectionalInBounds(runningIndex - dirOffset, runningIndex,
             dir) && mOthelloBoard[runningIndex] == mNextPlayer) {
               runningIndex += dirOffset; //update running index
            }//end while loop

            /* If your current location is your own piece, then you go back in
            the opposite direction and set the enemy's pieces to your own. You 
            also update the total value of the board, increment the switched 
            pieces, set the validMove variable to true, and add the flipset to 
            the move's vector of flipsets. */
            if (DirectionalInBounds(runningIndex - dirOffset, runningIndex, 
             dir) && mOthelloBoard[runningIndex] == currentPlayer) {
               while (mOthelloBoard[runningIndex -= dirOffset] == mNextPlayer) {
                  /* Update the board value to account for the piece no longer
                  belonging to the enemy. */
                  mValue -= mNextPlayer;
                  mOthelloBoard[runningIndex] = currentPlayer; //update board
                  switchedPieces += 1; //increment number of switched pieces
                  //update board value to account for the piece now being yours
                  mValue += currentPlayer;
               }//end while loop

               validMove = true; //update validMove
               /* Add a FlipSet to the move (to keep track of how many pieces 
               were captured and what direction you were traveling in. */
               m->AddFlipSet(OthelloMove::FlipSet(switchedPieces, dir));
            }//end if
         }//end if
      }//end for loop

      //if it was a valid move, then we push the move onto the back of the 
      //mHistory vector
      if (validMove)
         mHistory.push_back(m);

      /* If it was a valid move, then change the board location(board[mIndex])
      to your own piece, otherwise, keep the board location at 0, indicating  
      an invalid move. */
      mOthelloBoard[m->mIndex] = (validMove ? currentPlayer : 0);
      //update the value of the board accordingly
      mValue += (validMove ? currentPlayer : 0);
   }//end if
   
   //if the move is a pass, we add a flipset corresponding to a pass move
   if (m->IsPass()) {
      m->AddFlipSet(OthelloMove::FlipSet(0, 0));
      mHistory.push_back(m);
   }//end if
   
   //increment the pass count if the move was a pass or set it to 0 otherwise
   mPassCount = (m->IsPass() ? mPassCount + 1 : 0);
   //change the value of mNextPlayer accordingly
   mNextPlayer = (mNextPlayer == OTHELLO_P2 ? OTHELLO_P1 : OTHELLO_P2);
}//close ApplyMove(...)


/* Undoes the last move applied to the board, restoring it to the state it was
in prior to the most recent move (including whose turn it is, what the board 
value is, and what the pass count is). */
void OthelloBoard::UndoLastMove() {
   if (GetMoveCount() > 0) { 
      //set the currentPlayer variable with the appropriate value
      char currentPlayer = (mNextPlayer == OTHELLO_P2 ? OTHELLO_P1 : OTHELLO_P2);
      //set the deleteMove variable to the most recent move
      OthelloMove* deleteMove = (OthelloMove*)mHistory.back();
      //initialize the running index and the directional offset
      int runningIndex = 0, dirOffset = 0;
      //only delete moves that are not passes
      if (!deleteMove->IsPass()) {
         //change the piece back to unoccupied
         mOthelloBoard[deleteMove->mIndex] = 0;
         //decrement the board value according to the player
         mValue -= mNextPlayer;

         /* While there are flipsets in the move's flipset vector, iterate 
         through them and change the pieces back to their original values */
         for (vector<OthelloMove::FlipSet>::iterator itr = deleteMove->
          mFlips.begin(); itr != deleteMove->mFlips.end(); itr++) {
            //Initialize the directional offset and running index values
            dirOffset = GetDirectionalOffset(deleteMove->mIndex, itr->direction);
            runningIndex = deleteMove->mIndex + dirOffset;

            /* While there are still pieces that were switched, go one more 
            space in that direction and flip the pieces back to their original 
            values and update the board value at the same time */
            for (itr->switched; itr->switched != 0; itr->switched--) {             
               mValue -= mNextPlayer;
               mOthelloBoard[runningIndex] = currentPlayer;
               mValue += currentPlayer;
               runningIndex += dirOffset;
            }//end inner for loop	
         }//end outer for loop
      }//end if

      //update pass count
      mPassCount = (GetMoveCount() > 0 ? (deleteMove->IsPass() ? 1 : 0) : 0);

      /* Delete the move after you are done undoing it and pop it from the back
      of the mHistory vector */
      delete deleteMove;
      mHistory.pop_back();
      //set mNextPlayer to the appropriate value / update mPassCount accordingly
      mNextPlayer = (mNextPlayer == OTHELLO_P2 ? OTHELLO_P1 : OTHELLO_P2);
   }//end if
}//close UndoLastMove()


/* Returns TRUE if the game is finished.  */
bool OthelloBoard::IsFinished() const {
   return  mPassCount == 2;
}//close IsFinished()


/* Returns the current player score given a specified player. This score is 
calculated by iterating through the game board and keeping track of how many 
game pieces belong to the specified player */
int OthelloBoard::GetPlayerScore(char player) const {
   int playerScore = 0;
   //iterate through the board and update the player score
   for (int i = 0; i < OTHELLO_BOARD_SIZE; i++)
      playerScore += (mOthelloBoard[i] == player) ? 1 : 0;
   return playerScore;
}//close GetPlayerScore(...)


/* Undoes any active move on the Othello board and resets it to it's initial
un-played state */
void OthelloBoard::ResetGameBoard() {
   int appliedMoveCount = GetMoveCount();
   //undo all applied moves and reset game board values to 0
   if (appliedMoveCount > 0) {
      for (int i = 0; i < appliedMoveCount; i++)
         UndoLastMove();
   }//end if
   //Finally, reset board value, pass count, and the next player
   mValue = 0, mPassCount = 0;
   mNextPlayer = OTHELLO_P2;
}//close ResetGameBoard()


/* Populates a given vector with the indexes of the winning player's game 
pieces. Only called when the game is finished. */
void OthelloBoard::UpdateWinningCells(vector<int>* list){
   //make sure that the board is finished
   if (IsFinished()) { 
      int boardVal = GetValue(); //so we know which values to add to our list
      //iterate through the game board
      for (int i = 0; i < OTHELLO_BOARD_SIZE; i ++) {
         //add the right values to the back of the vector
         if (boardVal > 0) {
            //Player 1 won
            if (mOthelloBoard[i] == 1)
               list->push_back(i);
         }//end if
         else if (boardVal < 0) {
            //Player 2 won
            if (mOthelloBoard[i] == -1)
               list->push_back(i);
         } else {
            //board value is equal to 0 (It's a tie)
            if(mOthelloBoard[i] != 0)
               list->push_back(i);
         }//end else
      }//end for
   }//end if
}//close UpdateWinningCells(...)


/* Returns the value of the game board at a specified index */
char OthelloBoard::GetBoardValueAtIndex(int index) {
   return (index >= 0 && index < OTHELLO_BOARD_SIZE) ? 
    mOthelloBoard[index] : 0;
}//close GetBoardValueAtIndex(...)


/* Determines whether traveling one space in a specified direction is still 
in bounds on the game board (Note: this is necessary because the game board
is stored as a 1-dimensional array, which makes it much harder to gauge whether 
the space in bounds or not). 
Note: index1 is the original index and index2 is the new index (which is 
obtained by using GetDirectionalOffset()) */
bool OthelloBoard::DirectionalInBounds(int index1, int index2, int direction) 
 const {
   //make sure that both indexes are within the array, otherwise return false
   if (InBounds(index1) && InBounds(index2)) {
      bool isInBounds = FALSE;
      /* Make sure that both indexes are following the same pattern. If that
      pattern is broken, that means that the given indexes are no longer in 
      bounds for the particular direction. */
      switch (direction) {
         case 0: //North-West Direction In-Bounds Check (NW)
            isInBounds = (index2 % 8 == (index1 % 8) - 1) ? TRUE : FALSE;    
            break;
         case 1: //North Direction In-Bounds Check (N)
            isInBounds = (index2 % 8 == index1 % 8) ? TRUE : FALSE;
            break;
         case 2: //North-East Direction In-Bounds Check (NE)
            isInBounds = (index2 % 8 == (index1 % 8) + 1) ? TRUE : FALSE;
            break;
         case 3: //East Direction In-Bounds Check (E)
            isInBounds = (index2 % 8 == (index1 % 8) + 1) ? TRUE : FALSE;
            break;
         case 4: //South-East Direction In-Bounds Check (SE)
            isInBounds = (index2 % 8 == (index1 % 8) + 1) ? TRUE : FALSE;
            break;
         case 5: //South Direction In-Bounds Check (S)
            isInBounds = (index2 % 8 == index1 % 8) ? TRUE : FALSE;
            break;
         case 6: //South-West Direction In-Bounds Check (SW)
            isInBounds = (index2 % 8 == (index1 % 8) - 1) ? TRUE : FALSE;
            break;
         case 7: //West Direction In-Bounds Check (W)
            isInBounds = (index2 % 8 == (index1 % 8) - 1) ? TRUE : FALSE;
            break;
         default: //No Direction is Given
            isInBounds = FALSE; //not in bounds by default
            break;
      }//end switch()
      return isInBounds;
   }//end if
   else
      return FALSE;
}//close DirectionalInBounds(...)


/* Returns the appropriate offset value based on what direction is being
traversed (Note: this is necessary because the game board is stored as a
1-dimensional array). */
int OthelloBoard::GetDirectionalOffset(int index, int direction) const {
   int offsetDirection = 0;
   switch (direction) {
      case 0: //Moving in the North-Direction (NW)
         //we move backward 9 indexes in the array to signify travelling NW
         offsetDirection = -9; 
         break;  
      case 1: //Moving in the North Direction (N)
         //we move backward 8 indexes in the array to signify travelling N
         offsetDirection = -8; 
         break;
      case 2: //Moving in the North-East Direction (NE)
         //we move backward 9 indexes in the array to signify travelling NE
         offsetDirection = -7; 
         break;
      case 3: //Moving in the East Direction (E)
         //we move forward 1 index in the array to signify travelling E
         offsetDirection = 1; 
         break;
      case 4: //Moving in the South-East Direction (SE)
         //we move forward 9 indexes in the array to signify travelling SE
         offsetDirection = 9; 
         break;
      case 5: //Moving in the South Direction (S)
         //we move forward 8 indexes in the array to signify travelling S
         offsetDirection = 8; 
         break;
      case 6: //Moving in the South-West Direction (SW)
         //we move forward 7 indexes in the array to signify travelling SW
         offsetDirection = 7; 
         break;
      case 7: //Moving in the West Direction (W)
         //we move backward 1 index in the array to signify travelling W
         offsetDirection = -1; 
         break;
      default: //No Direction is Given
         //no direction is given, so we do not apply an offset to the index
         offsetDirection = 0; 
         break;
   }//end switch
   return offsetDirection;
}//close GetDirectionalOffset(...)