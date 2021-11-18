/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: GameView.h
*
* Description: Header file for the GameView class, which encompasses all
* of the neccessary components needed to display changes in the gameboard 
* and display them to the user.
* 
*/

#pragma once
#ifndef __GAMEVIEW_H
#define __GAMEVIEW_H
#include "GameBoard.h"

using namespace std;

class GameView {
public:
   /* Updates a given RECT with the endpoints of the area that encompasses the 
   game board. Returns TRUE if successfull and FALSE if unsucessful in 
   updating the RECT. */
   virtual BOOL GetGameBoardRect(HWND hWnd, RECT* pRect) const = 0;


   /* Draws a straight line to the window given a starting point and an ending 
   point. */
   virtual void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) const = 0;


   /* Returns the index of the cell that the user clicked on (assuming that 
   they clicked inside of the game board. If the area that was clicked was not 
   inside of the game board, then a value of -1 is returned. */
   virtual int GetCellNumFromPoint(HWND hWnd, int x, int y) const = 0;


   /* Updates a given RECT with the endpoints of the area that encompasses the 
   given cell index. Returns TRUE if successfull and FALSE if unsucessful in 
   updating the RECT. */
   virtual BOOL GetCellRect(HWND hWnd, int index, RECT* pRect) const = 0;


   /* Draws a given icon in  the middle of a specified RECT. */
   virtual void DrawCenteredIcon(HDC hdc, RECT* pRect, HICON hIcon) const = 0;


   /* Shades in the cell RECTs of all of the available moves that the player 
   can make. The avaialable move indexes are passed in using a vector. */
   virtual void ShowAvailableMoves(HWND hWnd, HDC hdc, vector<int> list, HBRUSH 
    brush) const = 0;


   /* Shades in the cell RECTs of all of the winning pieces on the game board. 
   The indexes of the winning pieces are passed in using a vector. In the case 
   of a tie, all of the pieces on the game board are shaded. */
   virtual void ShowWinningCells(HWND hWnd, HDC hdc, vector<int> list, HBRUSH 
    p1Brush, HBRUSH p2Brush, HBRUSH tieBrush, HICON p1Icon, HICON p2Icon) const = 0;


   /* Draws the current state of the game board including any occupied spaces and 
   each player's current score (if applicable). */
   virtual void DrawGameBoard(HWND hWnd, HDC hdc, HICON p1Icon, HICON p2Icon, HBRUSH 
    bkBrush, HFONT hFont) const = 0;


   /* Displays the current player's turn (or the winner if the game is over). */
   virtual void ShowPlayerTurn(HWND hWnd, HDC hdc, HFONT hFont) const = 0;

};	//close class GameView

#endif