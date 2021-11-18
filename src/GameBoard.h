/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: GameBoard.h
*
* Description: Header file for the GameBoard class, which encompasses all
* of the neccessary components that make up a game board.
* 
*/

#pragma once
#ifndef __GAMEBOARD_H
#define __GAMEBOARD_H

#include "GameMove.h"
#include <vector>

using namespace std;

class GameBoard {
public:
   /* Virtual destructors are important for class hierarchies.Add a virtual 
   destructor to all your classes; the destructor can be empty like this. */
   virtual ~GameBoard() {}


   /* Initializes a GameBoard with a NextPlayer of 1 and a Value of 0. */
   GameBoard() : mNextPlayer(1), mValue(0) {}


   /* Fills a vector with all possible GameMoves that can be applied to this
   board. The moves returned must be deleted by whoever calls the function. */
   virtual void GetPossibleMoves(vector<GameMove*>* list) const = 0;


   /* Applies a given move to the board, updating its game state and history
   accordingly. The board takes ownership of the move pointer given. The
   GameMove must an appropriate type for this GameBoard. */
   virtual void ApplyMove(GameMove* move) = 0;


   /* Undoes the last move on the board. */
   virtual void UndoLastMove() = 0;


   /* Creates an appropriate GameMove-derived object representing a "blank" move
   on this type of board. This object can be assigned to with an integer. */
   virtual GameMove* CreateMove() const = 0;


   /* Creates an appropriate GameMove-derived object representing an indexed move
  on this type of board. This object can be assigned to with an integer. */
   virtual GameMove* CreateMove(int index) const = 0;


   /* Returns true if the game board is "finished", e.g., if one player has won
   and no more moves should be allowed. */
   virtual bool IsFinished() const = 0;


   /* Undoes any active move on the game board and resets it to it's initial
   un-played state */
   virtual void ResetGameBoard() = 0;


   /* Returns the overall value of the game board */
   inline int GetValue() const { return mValue; }


   /* Returns the next player in the game */
   inline int GetNextPlayer() const { return mNextPlayer; }


   /* Populates a given vector with the indexes of the winning player's 
   game pieces. Only called when the game is finished. */
   virtual void UpdateWinningCells(vector<int>* list) = 0;


   /* Returns the value of the game board at a specified index */
   virtual char GetBoardValueAtIndex(int index) = 0;


   /* Returns the current player score given a specified player. This
   score is calculated by iterating through the game board and keeping
   track of how many game pieces belong to the specified player */
   virtual int GetPlayerScore(char player) const = 0;


   /* Gets a vector of all moves applied to this board. The last move in the
   vector is the most recent move applied to the board. */
   inline const vector<GameMove*>* GetMoveHistory() const {
      return &mHistory;
   }//close GetMoveHistory()


   /* Returns the number of moves that are currently appied to the game board.
   This is simply the size of the mHistory vector. */
   inline int GetMoveCount() const {
      return mHistory.size();
   }//close GetMoveCount()


protected:
   char mNextPlayer; //Keeps track of the next player in the game
   int mValue; //Keeps track of the game board's value
   vector<GameMove*> mHistory; //Keeps track of all moves applied to the board.
};

#endif