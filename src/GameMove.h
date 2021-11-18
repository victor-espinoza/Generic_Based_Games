/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: GameMove.h
*
* Description: Header file for the GameMove class, which encompasses all 
* of the neccessary components that make up a game move. 
* 
*/

#pragma once
#ifndef __GAMEMOVE_H
#define __GAMEMOVE_H

class GameMove {
public:
   /* Virtual destructors are important for class hierarchies.Add a virtual
   destructor to all your classes; the destructor can be empty like this. */
   virtual ~GameMove() {}


   /* Compares one GameMove to another, assuming they are of the same
   derived type. */
   virtual bool Equals(const GameMove& other) const = 0;


   /* Helper method for doing == comparison on derived GameMove types. This
   method will be called any time you use == to compare two GameMove objects;
   the operator will in turn call the virtual Equals method to support
   base-class comparisons. */
   friend bool operator==(const GameMove& lhs, const GameMove& rhs) {
      return lhs.Equals(rhs);
   }//close operator==(...)


   /* Returns the value of the derived game move (the game move's index) */
   virtual int GetValue() const = 0;
};	//close class GameMove


#endif