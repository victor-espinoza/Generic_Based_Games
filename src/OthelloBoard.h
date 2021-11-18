/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: OthelloBoard.h
*
* Description: Header file for the OthelloBoard class, which is a child class
* of Gameboard. Functions that are not implemented in the GameBoard class are 
* defined in this header file and implemented in the accompanying cpp file.
*
*/

#pragma once
#ifndef __OTHELLOBOARD_H
#define __OTHELLOBOARD_H

#include <vector>
#include "GameBoard.h"
#include "GameMove.h"
#include "OthelloMove.h"
#include "framework.h"

using namespace std;

const int OTHELLO_BOARD_SIZE = 64; //The size of an OthelloBoard
const int DIRECTIONS = 8; //NW, N, NE, E, SE, S, SW, W
const int COLS = 8;
const int P2_INIT_IND1 = 27; //Index for initial OthelloBoard piece 
const int P1_INIT_IND1 = 28; //Index for initial OthelloBoard piece 
const int P1_INIT_IND2 = 35; //Index for initial OthelloBoard piece 
const int P2_INIT_IND2 = 36; //Index for initial OthelloBoard piece 

/* An OthelloBoard encapsulates data needed to represent a single game of 
Othello. This includes the state of the board, tracking the current player, 
and keeping a history of moves on the board. */
class OthelloBoard : public GameBoard {

public:
   /* I use an enum to keep track of the different possible players. */
   enum Player { EMPTY = 0, OTHELLO_P1 = 1, OTHELLO_P2 = -1 };

   /* Default constructor initializes the board to its starting "new game" 
   state */
   OthelloBoard();


   /* Fills in a vector with all possible moves on the game board for the 
   current player. If no moves are possible, then a single OthelloMove 
   representing a PASS is put in the vector.
   Precondition: vector is empty.
   Postcondition: vector contains all valid moves for the current player. */
   virtual void GetPossibleMoves(vector<GameMove*>* list) const;


   /* Applies a valid move to the board, updating the board state accordingly.
   You may assume that this move is valid, and is consistent with the list
   of possible moves returned by GetPossibleMoves. */
   virtual void ApplyMove(GameMove* move);


   /* Undoes the last move applied to the board, restoring it to the state it 
   was in prior to the most recent move. */
   virtual void UndoLastMove();


   /* Creates an OthelloMove object on the heap. Whoever calls this method is
   responsible for managing the move's lifetime (or transferring that task to
   someone else.) */
   virtual GameMove* CreateMove() const { return new OthelloMove; }


   /* Creates an appropriate GameMove-derived object representing an indexed 
   move on this type of board. Whoever calls this method is responsible for 
   managing the move's lifetime (or transferring that task to someone 
   else.) */
   virtual GameMove* CreateMove(int index) const { 
      return new OthelloMove(index);
   }//close CreateMove(...)


   /* Determines whether a given index is in - bounds of the current game 
   board or not. */
   inline static bool InBounds(int index) {
      return index >= 0 && index < OTHELLO_BOARD_SIZE;
   }//close InBounds()


   /* Returns TRUE if the game is finished.  */
   bool IsFinished() const;


   /* Undoes any active move on the Othello board and resets it to it's 
   initial un-played state */
   void ResetGameBoard();


   /* Populates a given vector with the indexes of the winning player's
   game pieces. Only called when the game is finished. */
   void UpdateWinningCells(vector<int>* list);


   /* Returns the value of the game board at a specified index */
   char GetBoardValueAtIndex(int index);


   /* Returns the appropriate offset value based on what direction is 
   being traversed (Note: this is necessary because the game board is 
   stored as a 1-dimensional array). */
   int GetDirectionalOffset(int direction, int index) const; 


   /* Determines whether traveling one space in a specified direction 
   is still in bounds on the game board (Note: this is necessary because 
   the game board is stored as a 1-dimensional array, which makes it much 
   harder to gauge whether the space in bounds or not) . */
   bool DirectionalInBounds(int index1, int index2, int direction) const;


   /* Returns the current player score given a specified player. This
   score is calculated by iterating through the game board and keeping
   track of how many game pieces belong to the specified player */
   virtual int GetPlayerScore(char player) const;

private:
   //OthelloView is a friend so it can access the game board to display it.
   friend class OthelloView;

   char mOthelloBoard[OTHELLO_BOARD_SIZE]; //the game board
   int mPassCount; //the number of passes in the current game
};	//close class OthelloBoard
#endif