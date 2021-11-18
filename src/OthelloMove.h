/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: OthelloMove.h
*
* Description: Header file for the OthelloMove class, which is a child class
* of GameMove. Functions that are not implemented in the GameMove class are
* defined in this header file and implemented in the accompanying cpp file.
*
*/

#pragma once
#ifndef __OTHELLOMOVE_H
#define __OTHELLOMOVE_H
#include "GameMove.h"
#include <vector>

using namespace std;

/* An OthelloMove encapsulates a single potential move on an OthelloBoard. It
is represented internally by a 0-based index. */
class OthelloMove : public GameMove {
private:
   
   friend class OthelloBoard; //OthelloBoard is a friend so it can access mIndex.

   /* A FlipSet tracks a direction and # of pieces that got flipped when this
   move was applied. */
   struct FlipSet {
      FlipSet(char sw, char dir) : switched(sw), direction(dir) { }
      char switched;
      char direction;
   }; //close struct FlipSet

   int mIndex; //The index of the Othello Move

   /* I use a FlipSet vector to keep track of which directionand how many flips
   occur when moving in said direction. */
   vector<FlipSet> mFlips; 

   /* Default constructor : initializes this move as a PASS. */
   OthelloMove();


   /* 1-parameter constructor: initializes this move with the given index */
   OthelloMove(int index);


   /* Adds a FlipSet to the back of the mFlips vector. */
   void AddFlipSet(FlipSet set) { mFlips.push_back(set); }

public:
   /* Creates a new OthelloMove */
   static void* operator new(size_t sz) {
      return ::operator new(sz); 
   }//close new(...)


   /* Deletes the OthelloMove */
   static void operator delete(void* ptr, size_t sz) {
      ::operator delete(ptr);
   }//close delete(...)


   /* OthelloMove destructor */
   virtual ~OthelloMove() {}


   /* Copy Constructor (given an OthelloMove reference as an input) */
   OthelloMove(const OthelloMove& other);


   /* Assignment operator taking a const OthelloMove& object input. */
   OthelloMove& operator=(const OthelloMove& rhs);


   /* Compares two OthelloMove objects for equality. */
   virtual bool Equals(const GameMove& other) const;


   /* Returns true if the move represents a Pass.*/
   inline bool IsPass() const { return mIndex == -1; }


   /* Returns the index value of the OthelloMove */
   virtual int GetValue() const {
      return mIndex;
   }//close GetValue(...)


   /* Clones the OthelloMove */
   virtual GameMove* Clone() const {
      return new OthelloMove(*this);
   }//close *Clone()

};	//close class OthelloMove

#endif