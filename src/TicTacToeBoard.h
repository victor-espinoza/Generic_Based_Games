/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: TicTacToeBoard.h
*
* Description: Header file for the TicTacToeBoard class, which is a child class
* of GameBoard. Functions that are not implemented in the GameBoard class are
* defined in this header file and implemented in the accompanying cpp file.
*
*/

#pragma once
#ifndef __TICTACTOEBOARD_H
#define __TICTACTOEBOARD_H

#include <vector>
#include "TicTacToeMove.h"
#include "GameBoard.h"
#include "GameMove.h"
#include "framework.h"

using namespace std;

const int TTT_BOARD_SIZE = 9; //The size of a Tic-Tac-Toe board

/* A TicTacToeBoard encapsulates data needed to represent a single game of 
Tic-Tac-Toe. This includes the state of the board, tracking the current 
player, and keeping a history of moves on the board. */
class TicTacToeBoard : public GameBoard {

public:
   /* I use an enum to keep track of the different possible players. */
   enum Player { EMPTY = 0, TTT_P1 = 1, TTT_P2 = -1 };

   /* Default constructor initializes the board to its starting "new game" 
   state */
   TicTacToeBoard();


   /* Fills in a vector with all possible moves on the game board for the
   current player. If no moves are possible, then a single TicTacToeMove
   representing a PASS is put in the vector.
   Precondition: vector is empty.
   Postcondition: vector contains all valid moves for the current player. */
   virtual void GetPossibleMoves(vector<GameMove*>* list) const;


   /* Applies a valid move to the board, updating the board state 
   accordingly. You may assume that this move is valid, and is consistent
   with the list of possible moves returned by GetPossibleMoves. */
   virtual void ApplyMove(GameMove* move);


   /* Undoes the last move applied to the board, restoring it to the state 
   it was in prior to the most recent move. */
   virtual void UndoLastMove();


   /* Creates a TicTacToeMove object on the heap. Whoever calls this method 
   is responsible for managing the move's lifetime (or transferring that 
   task to someone else.) */
   virtual GameMove* CreateMove() const { return new TicTacToeMove; }


   /* Creates an appropriate GameMove-derived object representing an indexed 
   move on this type of board. Whoever calls this method is responsible for 
   managing the move's lifetime (or transferring that task to someone 
   else.) */
   virtual GameMove* CreateMove(int index) const { 
      return new TicTacToeMove(index); 
   }//close CreateMove(...)


   /* Determines whether a given index is in - bounds of the current game 
   board or not. */
   inline static bool InBounds(int index) {
      return index >= 0 && index < TTT_BOARD_SIZE;
   }//close InBounds()


   /* Returns TRUE if the game is finished. */
   bool IsFinished() const;


   /* Checks to see if the game is finished yet and updates the game board's 
   value accordingly. */
   int UpdateBoardValue();


   /* Undoes any active move on the Othello board and resets it to it's 
   initial un-played state */
   void ResetGameBoard();


   /* Populates a given vector with the indexes of the winning player's
   game pieces. Only called when the game is finished. */
   void UpdateWinningCells(vector<int>* list);


   /* Returns the value of the game board at a specified index */
   char GetBoardValueAtIndex(int index);


   /* Returns the current player score given a specified player. For 
   Tic-Tac-Toe, the player's score only changes when they have won the 
   game. */
   int GetPlayerScore(char player) const;

private:
   /* TicTacToeView is a friend so it can access the game board to display 
   it. */
   friend class TicTacToeView;

   char mTTTBoard[TTT_BOARD_SIZE]; //the game board
   int mPassCount; //the number of passes in the current game
};	//close class TicTacToeBoard
#endif
