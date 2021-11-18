/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: TicTacToeMove.cpp
*
* Description: CPP file for the TicTacToeMove class, which is a child class
* of GameMove. Functions that are not implemented in the GameMove class are
* implemented in this file.
*
*/

#include "TicTacToeMove.h"
#include "GameExceptions.h"

/* Default constructor : initializes this move as a PASS. */
TicTacToeMove::TicTacToeMove() : mIndex(-1) {

}//close TicTacToeMove() default constructor


/* 1-parameter constructor: initializes this move with the given index */
TicTacToeMove::TicTacToeMove(int index) : mIndex(index) {

}//close TicTacToeMove(...) 1-parameter constructor


/* Copy Constructor (given a TicTacToeMove reference as an input) */
TicTacToeMove::TicTacToeMove(const TicTacToeMove& other) : 
 mIndex(other.mIndex) {

}//close TicTacToeMove(...) copy constructor


/* Assignment operator taking a const TicTacToeMove& object input. */
TicTacToeMove& TicTacToeMove::operator=(const TicTacToeMove& other) {
   if (this == &other)
      return *this;
   mIndex = other.mIndex;
   return *this;
}//close operator=(...)


/* Compares two TicTacToeMove objects for equality (checks if indexes 
are the same). */
bool TicTacToeMove::Equals(const GameMove& other) const {
   TicTacToeMove& casted = (TicTacToeMove&)other;
   return mIndex == casted.mIndex;
}//close Equals(...) 

