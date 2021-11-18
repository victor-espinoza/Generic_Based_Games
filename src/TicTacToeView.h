/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: TicTacToeView.h
*
* Description: Header file for the TicTacToeView class, which is a child 
* class of GameView. Functions that are not implemented in the GameView class 
* are defined in this header file and implemented in the accompanying cpp file.
*
*/

#pragma once
#ifndef __TICTACTOEVIEW_H
#define __TICTACTOEVIEW_H
#include "TicTacToeBoard.h"
#include "GameView.h"

using namespace std;

/* An MVC "View" class for printing the model of the TicTacToeBoard. This 
class acts as a "wrapper" around a TicTacToe pointer and handles requests to
display the board to the user. Friend of TicTacToeBoard. */
class TicTacToeView : public GameView {

private:
   TicTacToeBoard* mTicTacToeBoard; // The actual TicTacToeBoard pointer.
   const int TTT_CELL_SIZE = 100; //The size of each cell in the game board
   const int TTT_COLUMNS = 3; //Howw many columns the game board has


public:
   /* Updates a given RECT with the endpoints of the area that encompasses
   the game board. Returns TRUE if successfull and FALSE if unsucessful in
   updating the RECT. */
   virtual BOOL GetGameBoardRect(HWND hWnd, RECT* pRect) const;


   /* Draws a straight line to the window given a starting point and an
   ending point. */
   virtual void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) const;


   /* Returns the index of the cell that the user clicked on (assuming that
   they clicked inside of the game board. If the area that was clicked was
   not inside of the game board, then a value of -1 is returned. */
   virtual int GetCellNumFromPoint(HWND hWnd, int x, int y) const;


   /* Updates a given RECT with the endpoints of the area that encompasses
   the given cell index. Returns TRUE if successfull and FALSE if
   unsucessful in updating the RECT. */
   virtual BOOL GetCellRect(HWND hWnd, int index, RECT* pRect) const;


   /* Draws a given icon in  the middle of a specified RECT. */
   virtual void DrawCenteredIcon(HDC hdc, RECT* pRect, HICON hIcon) const;


   /* Shades in the cell RECTs of all of the available moves that the player
   can make. The avaialable move indexes are passed in using a vector. */
   virtual void ShowAvailableMoves(HWND hWnd, HDC hdc, vector<int> list, HBRUSH 
    brush) const;


   /* Shades in the cell RECTs of all of the winning pieces on the game board.
   The indexes of the winning pieces are passed in using a vector. In the case
   of a tie, all of the pieces on the game board are shaded. */
   virtual void ShowWinningCells(HWND hWnd, HDC hdc, vector<int> list, HBRUSH 
    p1Brush, HBRUSH p2Brush, HBRUSH tieBrush, HICON p1Icon, HICON p2Icon) const;


   /* Draws the current state of the game board including any occupied spaces and
   each player's current score (if applicable). */
   virtual void DrawGameBoard(HWND hWnd, HDC hdc, HICON p1Icon, HICON p2Icon, HBRUSH 
    bkBrush, HFONT hFont) const;


   /* Displays the current player's turn (or the winner if the game is over). */
   virtual void ShowPlayerTurn(HWND hWnd, HDC hdc, HFONT hFont) const;


   /* Casts the game board into a TicTacToeBoard object and initializes the 
   TicTacToeView so that it uses the casted game board and can access it's 
   data. */
   TicTacToeView(GameBoard* b) : mTicTacToeBoard((TicTacToeBoard*)b) {}
};	//close class TicTacToeView
#endif