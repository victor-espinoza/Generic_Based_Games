/*
* Victor Espinoza
* Created October 2021
* Project: Generic_Based_Games
*
* File Name: Generic_Based_Games.cpp
*
* Description: Defines the entry point for the application and runs the
* main code loop.
* 
*/

#include "Generic_Based_Games.h"

/* Global Variables: */
HINSTANCE hInst; //current instance
WCHAR szTitle[MAX_LOADSTRING]; //The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; //the main window class name
HWND tttBtn = NULL; //TicTacToe Button
HWND othBtn = NULL; //Othello Button
const int OTH_X_MIN = 675; //minimum size of the client window needed
const int OTH_Y_MIN = 600; //minimum size of the client window needed
const int TTT_X_MIN = 520; //minimum size of the client window needed
const int TTT_Y_MIN = 500; //minimum size of the client window needed
const int PASS = -1; //used when creating pass moves
const int BTN_HEIGHT = 100; //height of the buttons
const int BTN_WIDTH = 50; //width of the buttons

//Brushes, Icons, and Fonts
HBRUSH redBrush, blueBrush, lightGrayBrush, seaGreenBrush, 
 lightPurpleBrush, creamBrush;
HICON tttP1Icon, tttP2Icon, othelloP1Icon, othelloP2Icon;
HFONT hFont;

//Game variables
const int P1 = 1, P2 = -1; //used to easily distinguish between the players
bool moveIsValid; //used to determine validity of move for move command
bool winningCellsUpdated = FALSE; //used for winning pop-up screen
bool autoGameComplete = FALSE; //keeps track of if the game completes itself
int gameSelected = 0; //0 = None, 1 = Tic-Tac-Toe, 2 = Othello

GameBoard* board; //the state of the game board
GameView* view; //a View for displaying the game board
GameMove* playerMove; //creates and holds the user's desired move

vector<GameMove*> possMoves; // a holder for possible moves
vector<int> possMoveIndexes; //holds the indexes of the possible moves
vector<int> winningCellIndexes; //holds the indexes of the winning cells

/* Forward declarations of functions included in this code module: */
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


/* Initializes variables and performs the main message loop for the program */ 
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
 _In_opt_ HINSTANCE hPrevInstance,
 _In_ LPWSTR    lpCmdLine,
 _In_ int       nCmdShow) {
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(lpCmdLine);

   //Initialize global strings
   LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
   LoadStringW(hInstance, IDC_GENERICBASEDGAMES, szWindowClass, MAX_LOADSTRING);
   MyRegisterClass(hInstance);

   //Perform application initialization:
   if (!InitInstance(hInstance, nCmdShow))
      return FALSE;

   HACCEL hAccelTable = LoadAccelerators(hInstance, 
    MAKEINTRESOURCE(IDC_GENERICBASEDGAMES));

   MSG msg;

   //Main message loop:
   while (GetMessage(&msg, nullptr, 0, 0)) {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }//end if
   }//end while

   return (int)msg.wParam;
}//close  wWinMain(...)


/* Registers the window class. */
ATOM MyRegisterClass(HINSTANCE hInstance) {
   WNDCLASSEXW wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GENERICBASEDGAMES));
   wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
   wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GENERICBASEDGAMES);
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

   return RegisterClassExW(&wcex);
}//close MyRegisterClass(...)


/* This function saves the instance handle in a global variable and
creates/s the main program window. */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance; // Store instance handle in our global variable
   //create the main program window
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, TTT_X_MIN, TTT_Y_MIN, nullptr, nullptr,
    hInstance, nullptr);
   //return false if the window was not created successfully
   if (!hWnd)
      return FALSE;
   //display/update the main program window
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}//close InitInstance(...)


/* Adds othello and Tic-Tac-Toe buttons to the program */
void AddButtons(HWND hWnd) {
   RECT rc;
   if (GetClientRect(hWnd, &rc)) {
      //center the button position so that it is in the middle of the screen.
      int center_x = rc.left + ((rc.right - rc.left) - BTN_WIDTH) / 2;
      int center_y = rc.top + ((rc.bottom - rc.top) - BTN_HEIGHT) / 2;
      tttBtn = CreateWindowW(L"Button", L"Tic Tac Toe", WS_VISIBLE | WS_CHILD, 
       center_x, center_y, BTN_HEIGHT, BTN_WIDTH, hWnd, (HMENU)TTT_BTN, 
       NULL, NULL);
      //add offset to othBtn so that its not created on top of the tttBtn.
      othBtn = CreateWindowW(L"Button", L"Othello", WS_VISIBLE | WS_CHILD, 
       center_x, center_y + BTN_WIDTH, BTN_HEIGHT, BTN_WIDTH, hWnd, 
       (HMENU)OTH_BTN, NULL, NULL);
   }//end if
}//close AddButtons(...)


/* Creates a PASS move and applies it to the game board */
void PassTurn(HWND hWnd) {
   //make sure that a game is selected and that the game isn't finished.
   if (gameSelected > 0 && !board->IsFinished()) {
      //create a PASS move and apply it to the game board
      playerMove = board->CreateMove(PASS);
      board->ApplyMove(playerMove);
   }//end if  
}//close PassTurn(...)


/* Forces a paint message to the window */
void ForcePaintMsg(HWND hWnd) {
   //Force a paint message
   //Post WM_PAINT to the windowProc (It gets queued in the msg queue)
   InvalidateRect(hWnd, NULL, TRUE); 
   //Force immediate handling of WM_PAINT
   UpdateWindow(hWnd); 
}//close ForcePaintMsg(...)


/* There is no native way to center a message box to its parent window.
In order to achieve this, we need to create a hook and change the 
position of the dialog box when it is created. */
int MessageBoxCentered(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, 
 UINT uType) {
   //create and initialize the hook
   static HHOOK msgBoxHook{};
   msgBoxHook = SetWindowsHookEx(WH_CBT,
      [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
         //Hook Procedure
         if (nCode == HCBT_CREATEWND) {
            if (((LPCBT_CREATEWND)lParam)->lpcs->lpszClass == 
             (LPWSTR)(ATOM)32770) { // #32770 = dialog box class
               RECT rcParent{}; 
               // Retrieve the RECT of the parent class. 
               if (GetWindowRect(
                ((LPCBT_CREATEWND)lParam)->lpcs->hwndParent, &rcParent)) {
                  //change the X coordiante so its centered
                  ((LPCBT_CREATEWND)lParam)->lpcs->x = rcParent.left +
                   ((rcParent.right - rcParent.left) -
                   ((LPCBT_CREATEWND)lParam)->lpcs->cx) / 2;
                  //change the Y coordiante so its centered
                  ((LPCBT_CREATEWND)lParam)->lpcs->y = rcParent.top +
                   ((rcParent.bottom - rcParent.top) -
                   ((LPCBT_CREATEWND)lParam)->lpcs->cy) / 2;
               }//end if      
            }//end if
         }//end if
         return CallNextHookEx(msgBoxHook, nCode, wParam, lParam);
      }, 0, GetCurrentThreadId());

   //create the newly centered message box
   int msgBox { MessageBox(hWnd, lpText, lpCaption, uType) };
   //hook procedure is no longer needed, so release it
   UnhookWindowsHookEx(msgBoxHook);
   //return the message box value
   return msgBox; 
}//close MessageBoxCentered(...)


/* Handles all of the messages being sent to the main window */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, 
 LPARAM lParam) {
   switch (message) {
      case WM_CREATE: {
         //Create the different brushes used throughout the project
         redBrush = CreateSolidBrush(RGB(210, 39, 48)); 
         blueBrush = CreateSolidBrush(RGB(0, 105, 148)); 
         lightGrayBrush = CreateSolidBrush(RGB(192, 192, 192));
         seaGreenBrush = CreateSolidBrush(RGB(60, 179, 113)); 
         lightPurpleBrush = CreateSolidBrush(RGB(187, 142, 187));
         creamBrush = CreateSolidBrush(RGB(255, 253, 208));

         //Load Player Icons into project
         tttP1Icon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TTT_P1_ICON));
         tttP2Icon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TTT_P2_ICON));
         othelloP1Icon = LoadIcon(hInst, 
          MAKEINTRESOURCE(IDI_OTHELLO_P1_ICON));
         othelloP2Icon = LoadIcon(hInst, 
          MAKEINTRESOURCE(IDI_OTHELLO_P2_ICON));

         //Create font for displaying information
         hFont = CreateFont(25, 10, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
          DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Impact"); 

         //Finally, create the different buttons
         AddButtons(hWnd);
         break;
      }//end case WM_CREATE
      case WM_COMMAND: {
         int wmId = LOWORD(wParam);
         // Parse the menu selections:
         switch (wmId) {
            case IDM_ABOUT:
               DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
               break; //end case IDM_ABOUT
            case IDM_EXIT:
               DestroyWindow(hWnd);
               break; //end case IDM_EXIT
            case ID_FILE_NEWGAME: {
               if (gameSelected > 0) {
                  int startNewGame = MessageBoxCentered(hWnd, 
                   L"Are you sure you want to start a new game?", L"New Game", 
                   MB_YESNO | MB_ICONQUESTION);
                  if (startNewGame == IDYES) {
                     /* Get rid of any active moves on the game board and
                     re-initializes all of the variables */
                     board->ResetGameBoard();
                     winningCellIndexes.clear();
                     winningCellsUpdated = FALSE;
                     autoGameComplete = FALSE;
                     ForcePaintMsg(hWnd); //Force a paint message
                  }//end if
               }//end if
               break;
            }//end case ID_FILE_NEWGAME
            case ID_FILE_SELECTGAME: {
               if (gameSelected > 0) {
                  //Make sure the user wants to select the game mode
                  int selectGameMode = MessageBoxCentered(hWnd,
                   L"Are you sure you want to select the game mode?",
                   L"Select Game", MB_YESNO | MB_ICONQUESTION);
                  if (selectGameMode == IDYES && gameSelected > 0 && 
                   !board == NULL) {
                     /* Re-initializes important global variables */
                     gameSelected = 0;
                     winningCellsUpdated = FALSE;
                     autoGameComplete = FALSE;

                     /* Get rid of any active moves on the game board and
                     re-initializes all of the  game variables */
                     board->ResetGameBoard();
                     winningCellIndexes.clear();

                     /* Also delete the board and the view pointers that 
                     are on the heap */
                     delete board;
                     delete view;

                     //Make the buttons visible again
                     ShowWindow(tttBtn, SW_SHOW); //un-hide button from window
                     ShowWindow(othBtn, SW_SHOW); //un-hide button from window

                     ForcePaintMsg(hWnd); //Force a paint message
                  }//end if
               }//end if
               break;
            }//end case ID_FILE_SELECTGAME
            case ID_EDIT_UNDOMOVE: {
               if (gameSelected > 0 && board->GetMoveCount() > 0) {
                  /* check to see if the game was completed automatically. If  
                  it was, then call UndoLastMove() 3 times (Twice to account  
                  for both pass moves that ended the game, and 1 time to undo  
                  the previous move). */
                  if (autoGameComplete) {
                     board->UndoLastMove(); //Undo PASS move
                     board->UndoLastMove(); //Undo PASS move
                     board->UndoLastMove(); //Undo user's last move
                  } else
                     board->UndoLastMove();

                  //reset appropriate variables since the game isn't finished.
                  winningCellsUpdated = FALSE; 
                  autoGameComplete = FALSE; 
                  winningCellIndexes.clear();

                  ForcePaintMsg(hWnd); //Force a paint message
               }//end if  
               break;
            } //end case ID_EDIT_UNDOMOVE
            case TTT_BTN: {
               //Update the gameSelected variable
               gameSelected = 1;

               //Set board and view to appropriate TicTacToe types
               board = new TicTacToeBoard();
               view = new TicTacToeView(board);

               //Hide buttons from window
               ShowWindow(tttBtn, SW_HIDE); //Hide button from window
               ShowWindow(othBtn, SW_HIDE); //hide button from window

               ForcePaintMsg(hWnd); //Force a paint message
               break;
            }//end case TTT_BTN
            case OTH_BTN: {
               //Update the gameSelected variable
               gameSelected = 2;

               //Set board and view to appropriate TicTacToe types
               board = new OthelloBoard();
               view = new OthelloView(board);

               //Hide buttons from window
               ShowWindow(tttBtn, SW_HIDE); //Hide button from window
               ShowWindow(othBtn, SW_HIDE); //hide button from window

               /* Re-set the window position to avoid any drawing issues 
               caused between the different board game sizes (You can 
               make the window smaller while playing Tic-Tac-Toe than the 
               required size to draw the othello board, so the window may 
               have to be re-sized when switching between game types). */
               RECT rc;    
               if (GetWindowRect(hWnd, &rc)) {
                  int screenWidth = rc.right - rc.left;
                  int screenHeight = rc.bottom - rc.top;
                  SetWindowPos(hWnd, NULL, rc.left, rc.top, 
                   screenWidth < OTH_X_MIN ? CW_USEDEFAULT : screenWidth, 
                   screenHeight < OTH_X_MIN ? CW_USEDEFAULT : screenHeight,
                   SWP_NOZORDER);
               }//end if

               ForcePaintMsg(hWnd); //Force a paint message
               break;
            }//end case OTH_BTN
            case ID_EDIT_PASS: {  
               //Create and apply a PASS move to the game board.
               PassTurn(hWnd); 
               ForcePaintMsg(hWnd); //Force a paint message
               break;
            }//end case ID_EDIT_PASS
            default:
               return DefWindowProc(hWnd, message, wParam, lParam);
         }//end switch (wmId)  
         break;
      }//end case WM_COMMAND
      case WM_LBUTTONDOWN: {
         //Get the X and Y coordinates of the user's click
         int xPos = GET_X_LPARAM(lParam);
         int yPos = GET_Y_LPARAM(lParam);

         //Only do stuff if the game isn't finished yet
         if (board->IsFinished())
            break;
         /* Get the index on the game board of where the user 
         clicked (if applicable). */
         int index = view->GetCellNumFromPoint(hWnd, xPos, yPos);

         HDC hdc = GetDC(hWnd);
         if (hdc != NULL && gameSelected) {
            /* Make sure the index is valid (that the user clicked 
            inside of the game board). */
            if (index != -1) {
               RECT rcCell;
               try {
                  //Try creating a game move for the given index
                  playerMove = board->CreateMove(index);
                  //Check to see if the move is valid
                  board->GetPossibleMoves(&possMoves);
                  moveIsValid = false;
                  /* Iterate through the possible movesand check to
                  //see if the user move matches any of them. */
                  for (vector<GameMove*>::iterator itr = 
                   possMoves.begin(); itr != possMoves.end(); itr++) {
                     moveIsValid = ((**itr == *playerMove) ? true : 
                      moveIsValid);
                     //delete itr object since it is no longer needed
                     delete* itr; 
                  }//end for 
                  //Clear possMoves since it is no longer needed
                  possMoves.clear();

                  //If move is valid, apply the move to the board
                  if (moveIsValid) {
                     //Apply move to the game board
                     board->ApplyMove(playerMove);
                     //Display applied move 
                     if (view->GetCellRect(hWnd, index, &rcCell)) {
                        view->DrawCenteredIcon(hdc, &rcCell, 
                         (gameSelected == 1) ? (
                         (board->GetNextPlayer() == P2) ?  
                         tttP2Icon : tttP1Icon) : (
                         (board->GetNextPlayer() == P2) ? 
                         othelloP2Icon : othelloP1Icon));

                        ForcePaintMsg(hWnd); //Force a paint message
                     }//end if

                     /* Now that the move is applied, check to see 
                     what the next player's move options are. If the 
                     next player's only possible move is to pass, 
                     then go ahead and apply the passing move to the 
                     game board. */
                     board->GetPossibleMoves(&possMoves);
                     /* If the player's only option is to pass, then 
                     automatically do it for them. */
                     if (possMoves.size() == 1 && 
                      possMoves[0]->GetValue() == -1) {
                        //Apply a passing turn to the game board
                        PassTurn(hWnd);
                        //clear possMoves; its no longer needed
                        possMoves.clear();

                        /* Repeat the same process. This allows the game 
                        to end automatically if there are no moves left 
                        on the game board (Remember that the game is 
                        finished when both players pass their turn 
                        consecutively). */
                        board->GetPossibleMoves(&possMoves);
                        /* If the player's only option is to pass, then 
                        we automatically do it for them. */
                        if (possMoves.size() == 1 && 
                         possMoves[0]->GetValue() == -1) {
                           //Apply a passing turn to the game board
                           PassTurn(hWnd);
                           //clear possMoves; its no longer needed
                           possMoves.clear();

                           /* Update the autoGameComplete flag (Note:
                           this variable is important if the user 
                           choses to undo a move after a game 
                           completes itself automatically. Since the 
                           user didn't instruct the program to create 
                           those pass moves, I get rid of them and the 
                           user's previous move if they chose the undo 
                           move menu item). */
                           if (board->IsFinished()) 
                              autoGameComplete = TRUE;                       
                        }//end if                    
                     }//end if
                     //clear possMoves since it is no longer needed.
                     possMoves.clear();
                     ForcePaintMsg(hWnd); //Force a paint message               
                  }//end if
                  else
                     //if the move isn't valid, then delete it.
                     delete playerMove;
               }//end try
               catch (GameException& e) {
                  //If there was an exception, display the message and 
                  //delete the move.
                  MessageBoxA(hWnd, e.GetMessage().c_str(), 
                   "Exception Encountered", MB_OK | MB_ICONEXCLAMATION);
                  delete playerMove;
               }//end catch
            }//end if    
            ReleaseDC(hWnd, hdc); //release hdc; we are done using it
         }//end if
         break;
      }//end case WM_LBUTTONDOWN
      case WM_GETMINMAXINFO: {
         //Get the MinMax info
         MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;
         if (gameSelected == 2) {
            //Set the Minimum Size for the Window (Othello)
            pMinMax->ptMinTrackSize.x = OTH_X_MIN;
            pMinMax->ptMinTrackSize.y = OTH_Y_MIN;
         } else {
            //Set the Minimum Size for the Window (Tic-Tac-Toe)
            pMinMax->ptMinTrackSize.x = TTT_X_MIN;
            pMinMax->ptMinTrackSize.y = TTT_Y_MIN;
         }//end else
         break;
      }//end case WM_GETMINMAXINFO
      case WM_SIZE: {
         //Re-position the game mode buttons
         RECT rc;
         if (GetClientRect(hWnd, &rc)) {
            //Get the new center point of the window
            int center_x = rc.left + 
             ((rc.right - rc.left) - BTN_WIDTH) / 2;
            int center_y = rc.top + 
             ((rc.bottom - rc.top) - BTN_HEIGHT) / 2;
            //Adjust the postion of the buttons accordingly
            SetWindowPos(tttBtn, NULL, center_x, center_y, BTN_HEIGHT, 
             BTN_WIDTH, SWP_NOZORDER);
            SetWindowPos(othBtn, NULL, center_x, center_y + BTN_WIDTH, 
             BTN_HEIGHT, BTN_WIDTH, SWP_NOZORDER);
         }//end if
         break;
      }//end case WM_SIZE
      case WM_PAINT: {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint(hWnd, &ps);
         if (gameSelected) {
            //Draw The Game Board on the screen
            if (board->IsFinished() && !winningCellsUpdated) {
               /* Update the winning cells only once */
               winningCellsUpdated = true; //set to true 
               //update the winning cell indexes
               board->UpdateWinningCells(&winningCellIndexes); 
               ForcePaintMsg(hWnd); //Force a paint message
               /*//Un-comment to add a message box when the game ends.
               if (board->GetValue() == 0)
                  //its a draw
                  MessageBoxCentered(hWnd, L"Its a Draw",
                   L"Game Finished ", MB_OK | MB_ICONEXCLAMATION);
               else {
                  MessageBoxCentered(hWnd, board->GetValue() > 0 ?
                   L"Player 1 is the winner" : L"Player 2 is the winner",
                   L"Winner detected ", MB_OK | MB_ICONINFORMATION);
               }//end else    
               */
            }//end if

            //Draw the game board on the screen
            view->DrawGameBoard(hWnd, hdc, 
             ((gameSelected == 1) ?  tttP1Icon : othelloP1Icon),
             ((gameSelected == 1) ? tttP2Icon : othelloP2Icon), 
             ((gameSelected == 1) ? creamBrush : seaGreenBrush), hFont);

            //Show the player their available moves
            if (!board->IsFinished()) {
               //Populate the possible indexes vector
               board->GetPossibleMoves(&possMoves);
               for (vector<GameMove*>::iterator itr = possMoves.begin();
                  itr != possMoves.end(); itr++) {
                  possMoveIndexes.push_back((*itr)->GetValue());
                  delete* itr;
               }//end for 
               //Clear possMoves since it is no longer needed
               possMoves.clear();
               //Shade in all available spaces that can be selected.
               view->ShowAvailableMoves(hWnd, hdc, possMoveIndexes, 
                (gameSelected == 1 ? creamBrush: lightGrayBrush));
               //Clear the possMovesIndexes vector's contents 
               possMoveIndexes.clear();
            }//end if
            else {
               //The game is finished
               //Display the winner (if applicable)
               view->ShowPlayerTurn(hWnd, hdc, hFont); 
               //Show the indexes of the winning cells
               view->ShowWinningCells(hWnd, hdc, winningCellIndexes, 
                blueBrush, redBrush, lightPurpleBrush, 
                ((gameSelected == 1) ? tttP1Icon : othelloP1Icon),
                ((gameSelected == 1) ? tttP2Icon : othelloP2Icon));
            }//end else
         }//end if
         EndPaint(hWnd, &ps);
         break;
      }//end case WM_PAINT
      case WM_DESTROY: {
         //Dispose of Brushes and font
         DeleteObject(redBrush);
         DeleteObject(blueBrush);
         DeleteObject(lightGrayBrush);
         DeleteObject(seaGreenBrush);
         DeleteObject(lightPurpleBrush);
         DeleteObject(creamBrush);
         DeleteObject(hFont);

         /* Now that the game is done, delete all of the moves that 
         are still applied on the board. */
         int undoLimit = board->GetMoveCount();
         for (int i = 0; i < undoLimit; i++)
            board->UndoLastMove();

         /* Delete the boardand the view pointers that were put
         on the heap. */
         delete board;
         delete view;

         //Dispose of Icon images
         DestroyIcon(tttP1Icon);
         DestroyIcon(tttP2Icon);
         DestroyIcon(othelloP1Icon);
         DestroyIcon(othelloP2Icon);

         //Finally, post a quit message
         PostQuitMessage(0);
         break;
      }//end case WM_DESTROY
      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
   }//end switch (message)
   return 0;
}//close WndProc(...)


/* Message handler for about box. */
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, 
 LPARAM lParam) {
   UNREFERENCED_PARAMETER(lParam);
   switch (message) {
      case WM_INITDIALOG:
         return (INT_PTR)TRUE;
      case WM_COMMAND:
         if (LOWORD(wParam) == IDOK || 
          LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
         }//end if
         break;
      defualt: return (INT_PTR)FALSE;
   }//end switch (message)
   return (INT_PTR)FALSE;
}//close About(...)
