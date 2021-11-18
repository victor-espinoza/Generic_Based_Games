/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: OthelloView.cpp
*
* Description: CPP file for the OthelloView class, which is a child class
* of GameView. Functions that are not implemented in the GameView class are
* implemented in this file.
*
*/

#include "OthelloView.h"


/* Updates a given RECT with the endpoints of the area that encompasses
the game board. Returns TRUE if successfull and FALSE if unsucessful in
updating the RECT. */
BOOL OthelloView::GetGameBoardRect(HWND hWnd, RECT* pRect) const {
   RECT rc;
   if (GetClientRect(hWnd, &rc)) {
      //get the height and width of the game board
      int width = rc.right - rc.left;
      int height = rc.bottom - rc.top;
      //assign correct values to the RECT
      pRect->left = (width - OTH_CELL_SIZE * OTH_COLUMNS) / 2;
      pRect->top = (height - OTH_CELL_SIZE * OTH_COLUMNS) / 2;
      pRect->right = pRect->left + OTH_CELL_SIZE * OTH_COLUMNS;
      pRect->bottom = pRect->top + OTH_CELL_SIZE * OTH_COLUMNS;
      return TRUE;
   }//end if

   //In case of failure, initialize the Rectangle to empty and return FALSE
   SetRectEmpty(pRect);
   return FALSE;
}//close GetGameBoardRect(...)


/* Draws a straight line to the window given a starting point and an
ending point. */
void OthelloView::DrawLine(HDC hdc, int x1, int y1, int x2, int y2) const {
   //Draw A line given a starting point and an eding point
   MoveToEx(hdc, x1, y1, NULL);
   LineTo(hdc, x2, y2);
}//close DrawLine(...)


/* Returns the index of the cell that the user clicked on (assuming that
they clicked inside of the game board. If the area that was clicked was
not inside of the game board, then a value of -1 is returned. */
int OthelloView::GetCellNumFromPoint(HWND hWnd, int x, int y) const {
   RECT rc;
   POINT pt = { x, y };
   if (GetGameBoardRect(hWnd, &rc)) {
      //check if point is located inside of game board
      if (PtInRect(&rc, pt)) {
         //click was located within the game board
         //Initialize point values
         x = pt.x - rc.left;
         y = pt.y - rc.top;
         //get row/column of click
         int column = x / OTH_CELL_SIZE;
         int row = y / OTH_CELL_SIZE;
         //convert row/column to an index (0 - 63)
         return column + (row * OTH_COLUMNS);
      }//end if
   }//end if
   //click was not inside of the game board or an error occured
   return -1; 
}//close GetCellNumFromPoint(...)


/* Updates a given RECT with the endpoints of the area that encompasses
the given cell index. Returns TRUE if successfull and FALSE if
unsucessful in updating the RECT. */
BOOL OthelloView::GetCellRect(HWND hWnd, int index, RECT* pRect) const {
   //initialize pRect to an empty rectangle
   SetRectEmpty(pRect);
   //make sure index is valid
   if (index < 0 || index > OTHELLO_BOARD_SIZE - 1)
      return FALSE;
   RECT rcBoard;
   if (GetGameBoardRect(hWnd, &rcBoard)) {
      //Convert index from 0 to 63 into (x,y) pair
      int y = index / OTH_COLUMNS; //row number
      int x = index % OTH_COLUMNS; //column number
      //assign the correct cell values to our rectangle
      pRect->left = rcBoard.left + x * OTH_CELL_SIZE + 1;
      pRect->top = rcBoard.top + y * OTH_CELL_SIZE + 1;
      /* The CELL_SIZE is fixed, so in order to get the right and bottom
      values, we just add the CELL_SIZE value to the left and the top */
      pRect->right = pRect->left + OTH_CELL_SIZE - 1;
      pRect->bottom = pRect->top + OTH_CELL_SIZE - 1;
      //The cell rectangle was succesfully initialized, so return TRUE
      return TRUE;
   }//end if
   //return FALSE if unsuccessful
   return FALSE;
}//close GetCellRect(...)


/* Draws a given icon in  the middle of a specified RECT. */
void OthelloView::DrawCenteredIcon(HDC hdc, RECT* pRect, HICON hIcon) 
 const {
   //get the width and height of the icon
   const int ICON_WIDTH = GetSystemMetrics(SM_CXICON);
   const int ICON_HEIGHT = GetSystemMetrics(SM_CYICON);
   //make sure the given rectangle isn't null
   if (pRect != NULL) {
      //get the center of the rectangle
      int center_x = pRect->left + 
       ((pRect->right - pRect->left) - ICON_WIDTH) / 2;
      int center_y = pRect->top + 
       ((pRect->bottom - pRect->top) - ICON_HEIGHT) / 2;
      //draw the centered icon
      DrawIcon(hdc, center_x, center_y, hIcon);
   }//end if
}//close DrawCenteredIcon(...)


/* Shades in the cell RECTs of all of the available moves that the player
can make. The avaialable move indexes are passed in using a vector. */
void OthelloView::ShowAvailableMoves(HWND hWnd, HDC hdc, 
 vector<int> list, HBRUSH brush) const {
   RECT cellRect;
   //iterate through the list and shade in the cells.
   for (vector<int>::iterator itr = list.begin();
      itr != list.end(); itr++) {
      if (GetCellRect(hWnd, *itr, &cellRect))  
         FillRect(hdc, &cellRect, brush); //shade cell
   }//end for
}//close ShowAvailableMoves(...)


/* Shades in the cell RECTs of all of the winning pieces on the game board.
The indexes of the winning pieces are passed in using a vector. In the case
of a tie, all of the pieces on the game board are shaded. */
void OthelloView::ShowWinningCells(HWND hWnd, HDC hdc, 
 vector<int> list, HBRUSH p1Brush, HBRUSH p2Brush, HBRUSH tieBrush, 
 HICON p1Icon, HICON p2Icon) const {
   //Fill in the winning cells
   if (mOthelloBoard->IsFinished()) {
      RECT cellRect;
      int boardVal = mOthelloBoard->GetValue();
      int indVal = 0;
      //iterate through the list and shade in the winning cells
      for (vector<int>::iterator itr = list.begin();
         itr != list.end(); itr++) {
         if (GetCellRect(hWnd, *itr, &cellRect)) {
            indVal = mOthelloBoard->GetBoardValueAtIndex(*itr);
            //shade cell
            FillRect(hdc, &cellRect, (boardVal > 0) ? p1Brush : 
             ((boardVal < 0) ? p2Brush : tieBrush));
            //re-draw the icon on top of the newly shaded cell
            DrawCenteredIcon(hdc, &cellRect, (indVal == 1) ? 
             p1Icon : p2Icon);
         }//end if
      }//end for
   }//end if
}//close ShowWinningCells(...)


/* Draws the current state of the game board including any occupied spaces and
each player's current score (if applicable). */
void OthelloView::DrawGameBoard(HWND hWnd, HDC hdc, HICON p1Icon, 
 HICON p2Icon, HBRUSH bkBrush, HFONT hFont) const {
   RECT rc;
   if (GetGameBoardRect(hWnd, &rc)) {
      //Draw Player names and current turn inside of the client window
      RECT rcClient;
      if (GetClientRect(hWnd, &rcClient)) {
         WCHAR p1Text[50];
         WCHAR p2Text[50];
         SelectObject(hdc, hFont);
         //Set background to be gray and the p1 text to be blue
         SetBkColor(hdc, RGB(169, 169, 169)); //gray background
         SetBkMode(hdc, OPAQUE);
         SetTextColor(hdc, RGB(0, 78, 255)); //blue-colored text
         //print player 1 icon and the score
         wsprintf(p1Text, L" Player 1:  %d ", 
          mOthelloBoard->GetPlayerScore(1));
         TextOut(hdc, 10, 15, p1Text, lstrlen(p1Text));
         DrawIcon(hdc, 35, 45, p1Icon);
         //Set the p2 text to be red
         SetTextColor(hdc, RGB(171, 35, 40));
         //print player 2 icon and the score
         wsprintf(p2Text, L" Player 2:  %d ", 
          mOthelloBoard->GetPlayerScore(-1));
         TextOut(hdc, rcClient.right - 130, 15, p2Text,
          lstrlen(p2Text));
         DrawIcon(hdc, rcClient.right - 105, 45, p2Icon);
         //Display the current player's turn
         ShowPlayerTurn(hWnd, hdc, hFont);
      }//end if
      //fill game board background with pale green
      FillRect(hdc, &rc, bkBrush);
      //Draw the Othello Board lines
      for (int i = 0; i < OTH_COLUMNS + 1; i++) {
         //Draw Vertical Lines
         DrawLine(hdc, rc.left + (OTH_CELL_SIZE * i), rc.top, 
          rc.left + (OTH_CELL_SIZE * i), rc.bottom);
         //Draw Horizontal Lines
         DrawLine(hdc, rc.left, rc.top + (OTH_CELL_SIZE * i), 
          rc.right, rc.top + (OTH_CELL_SIZE * i));
      }//end for 

      //Draw all occupied cells
      RECT rcCell;
      char indexVal;
      for (int i = 0; i < OTHELLO_BOARD_SIZE; i++) {
         indexVal = mOthelloBoard->GetBoardValueAtIndex(i);
         if (indexVal != 0 && GetCellRect(hWnd, i, &rcCell)) {
            //fill in gameboard with player value
            DrawCenteredIcon(hdc, &rcCell, (indexVal == 1) ? 
             p1Icon : p2Icon);
         }//end if
      }//end for
   }//end if
}//close DrawGameBoard(...)


/* Displays the current player's turn (or the winner if the game is over). */
void OthelloView::ShowPlayerTurn(HWND hWnd, HDC hdc, HFONT hFont) const {
   /* Static variables only get allocated once instead of everytime you
   call the function */
   static const WCHAR p1TurnText[] = L" Player 1's Turn ";
   static const WCHAR p2TurnText[] = L" Player 2's Turn ";

   const WCHAR* pCurrTurnText = NULL;
   COLORREF cCurrTurnTextColor = RGB(0, 0, 0);
   int boardVal = 0;

   /* BLUE   - RGB(0, 78, 255);
      PURPLE - RGB(139, 0, 139);
      RED    - RGB(171, 35, 40)) */

   //get the right text to display
   if (!mOthelloBoard->IsFinished()) {
      pCurrTurnText = (mOthelloBoard->GetNextPlayer() == -1) ? 
       p1TurnText : p2TurnText;
      cCurrTurnTextColor = (mOthelloBoard->GetNextPlayer() == -1) ? 
       RGB(0, 78, 255) : RGB(171, 35, 40); 
   } else {
      boardVal = mOthelloBoard->GetValue();
      pCurrTurnText = (boardVal > 0) ? L" Player 1 Wins! " : 
       ((boardVal == 0) ? L" It's A Draw! " : L" Player 2 Wins! ");
      cCurrTurnTextColor = (boardVal > 0) ? RGB(0, 78, 255) : 
       ((boardVal == 0) ? RGB(139, 0, 139) : RGB(171, 35, 40));
   }//end else

   RECT rc, rcBoard;
   if (pCurrTurnText != NULL && GetClientRect(hWnd, &rc) && 
    GetGameBoardRect(hWnd, &rcBoard)) {
      /* Center the text in between the bottom of the game board and
      the remaining portion of the client rect */     
      rc.top = rcBoard.bottom + ((rc.bottom - rcBoard.bottom) / 3);
      //set the background to gray
      FillRect(hdc, &rc, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
      SelectObject(hdc, hFont);
      SetTextColor(hdc, cCurrTurnTextColor);
      SetBkColor(hdc, RGB(169, 169, 169));
      SetBkMode(hdc, OPAQUE);
      //SetBkMode(hdc, TRANSPARENT);
      //draw text on window
      DrawText(hdc, pCurrTurnText, lstrlen(pCurrTurnText), &rc, 
       DT_CENTER);
   }//end if
}//close ShowPlayerTurn(...)