/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: GameExceptions.h
*
* Description: Handles any exceptions that are thrown while a game
* is being played. Any exceptions are returned as string messages.
*
*/

#pragma once
#ifndef __GAMEEXCEPTIONS_H
#define __GAMEEXCEPTIONS_H

#include <string>

using namespace std;

class GameException {
protected:
   string mMessage;

/* Returns the exception message back to the user in the form of a string. */
public:
   GameException(const string& m) : mMessage(m) {}
   inline string GetMessage() const { return mMessage; }
}; //close class GameException


/* Returns the exception message back to the user in the form of a string. */
class OthelloException : public GameException {
public:
   OthelloException(const string& m) : GameException(m) {}
}; //close class OthelloException


/* Returns the exception message back to the user in the form of a string. */
class TicTacToeException : public GameException {
public:
   TicTacToeException(const string& m) : GameException(m) {}
};	//close class TicTacToeException

#endif