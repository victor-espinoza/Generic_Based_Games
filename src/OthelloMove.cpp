/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: OthelloMove.cpp
*
* Description: CPP file for the OthelloMove class, which is a child class
* of GameMove. Functions that are not implemented in the GameMove class are
* implemented in this file.
*
*/

#include "OthelloMove.h"
#include "GameExceptions.h"

/* Default constructor : initializes this move as a PASS. */
OthelloMove::OthelloMove() : mIndex(-1) {

}//close OthelloMove() default constructor


/* 1-parameter constructor: initializes this move with the given index */
OthelloMove::OthelloMove(int index) : mIndex(index) {

}//close OthelloMove(...) 1 - parameter constructor


/* Copy Constructor (given an OthelloMove reference as an input) */
OthelloMove::OthelloMove(const OthelloMove& other) : mIndex(other.mIndex) {

}//close OthelloMove(...) copy constructor


/* Assignment operator taking a const OthelloMove& object input. */
OthelloMove& OthelloMove::operator=(const OthelloMove& rhs) {
   if (this == &rhs)
      return *this;
   mIndex = rhs.mIndex;
   return *this;
}//close operator=(...)


/* Compares two OthelloMove objects for equality(if indexes are the 
same). */
bool OthelloMove::Equals(const GameMove& other) const {
   OthelloMove& casted = (OthelloMove&)other;
   return mIndex == casted.mIndex;
}//close Equals(...) 