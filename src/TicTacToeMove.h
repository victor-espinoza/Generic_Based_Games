/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: TicTacToeMove.h
*
* Description: Header file for the TicTacToeMove class, which is a child class
* of GameMove. Functions that are not implemented in the GameMove class are
* defined in this header file and implemented in the accompanying cpp file.
*
*/

#pragma once
#ifndef __TICTACTOEMOVE_H
#define __TICTACTOEMOVE_H
#include "GameMove.h"
#include <vector>

using namespace std;

/* A TicTacToeMove encapsulates a single potential move on a TicTacToeBoard. It
is represented internally by a 0-based index. */
class TicTacToeMove : public GameMove {
private:

   friend class TicTacToeBoard; //TicTacToeBoard is a friend so it can access mIndex. 

   int mIndex; //The index of the Tic-Tac-Toe Move

   
   /* Default constructor : initializes this move as a PASS. */
   TicTacToeMove();


   /* 1-parameter constructor: initializes this move with the given index */
   TicTacToeMove(int index);

public:
   /* Creates a new TicTacToeMove */
   static void* operator new(size_t sz) {
      return ::operator new(sz);
   }//close new(...)


   /* Deletes the TicTacToeMove */
   static void operator delete(void* ptr, size_t sz) {
      ::operator delete(ptr);
   }//close delete(...)


   /* TicTacToeMove Destructor */
   virtual ~TicTacToeMove() {}


   /* Copy Constructor (given a TicTacToeMove reference as an input) */
   TicTacToeMove(const TicTacToeMove&);


   /* Assignment operator taking a const TicTacToeMove& object input. */
   TicTacToeMove& operator=(const TicTacToeMove& rhs);


   /* Compares two TicTacToeMove objects for equality. */
   virtual bool Equals(const GameMove& other) const;


   /* Returns true if the move represents a Pass. */
   inline bool IsPass() const { return mIndex == -1; }


   /* Returns the index value of the TicTacToeMove */
   virtual int GetValue() const {
      return mIndex;
   }//close GetValue(...)


   /* Clones the TicTacToeMove */
   virtual GameMove* Clone() const {
      return new TicTacToeMove(*this);
   }//close *Clone()
};	//close class TicTacToeMove

#endif