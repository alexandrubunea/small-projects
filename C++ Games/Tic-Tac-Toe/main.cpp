#include <bits/stdc++.h>
#include <conio.h>

using namespace std;

// Helpers
#define ai    'x'
#define human 'o'
#define clear ' '
#define tie   'T'
#define xWin  'X' // alias a.i win
#define oWin  'O' // alias human win
#define nWin  'N' // Still have no winner or tie
#define pos   (char)254
#define hLine (char)179
#define vLine (char)196

// Variables
char gameBoard[3][3];
pair<int, int> score = make_pair(0, 0); // (int, int) = (AI Score, Human Score)
pair<int, int> selector = make_pair(1, 1);
bool isHumanTurn = true;

// Functions
short int minimax(short int alpha, short int beta, bool isMaximizing); // Minimax algorithm
void drawGameBoard(); // Draw the actual gameboard
void initGameBoard(); // Initialize the default game board
void resetGameBoard(); // Reset the game board to it's default values
void readKeyboard(); // Read data from keyboard
char checkWinner(); // Check if there's a winner
void nextMove(); // Calculate with minimax algorithm the next move of A.I


int main() {
  initGameBoard();
  drawGameBoard();
  while(true) {
    readKeyboard();
  }
  return 0;
}

void initGameBoard() {
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      gameBoard[i][j] = clear;
    }
  }
}
void resetGameBoard() { initGameBoard(); }
void drawGameBoard() {
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(selector.first == i && selector.second == j)
        cout << pos;
      else
        cout << gameBoard[i][j];
      if(j < 2)
        cout << hLine;
    }
    if(i < 2) {
      cout << "\n";
      for(int k = 0; k < 5; k++)
        cout << vLine;
      cout << "\n";
    }
  }
}
void readKeyboard() {
  if(isHumanTurn && kbhit()) {
    switch(getch()) {
      case 'w': {
        if(selector.first > 0)
          selector.first--;
        break;
      }
      case 'a': {
        if(selector.second > 0)
          selector.second--;
        break;
      }
      case 's': {
        if(selector.first < 2)
          selector.first++;
        break;
      }
      case 'd': {
        if(selector.second < 2)
          selector.second++;
        break;
      }
      case '\r': {
        if(gameBoard[selector.first][selector.second] != clear) {
          cout << "\nThis cell is already occupied.\n";
          sleep(1);
          break;
        }
        gameBoard[selector.first][selector.second] = human;
        system("cls");
        selector = make_pair(-1, -1);
        drawGameBoard();
        if(checkWinner() == oWin) {
          score.second++;
          cout << "\nGame has been won by human player.\n" << "Current score: A.I - " << score.first << " | Human - " << score.second << "\n";
          cout << "New round starting in 3 seconds...";
          sleep(3);
          resetGameBoard();
          selector = make_pair(1, 1);
          break;
        } else if(checkWinner() == nWin) {
          isHumanTurn = false;
          nextMove();
          if(checkWinner() == xWin) {
            system("cls");
            selector = make_pair(-1, -1);
            drawGameBoard();
            score.first++;
            cout << "\nGame has been won by A.I player.\n" << "Current score: A.I - " << score.first << " | Human - " << score.second << "\n";
            cout << "New round starting in 3 seconds...";
            sleep(3);
            resetGameBoard();
          }
          isHumanTurn = true;
          selector = make_pair(1, 1);
          break;
        } else if(checkWinner() == tie) {
          cout << "\nGame tie.\n" << "Current score: A.I - " << score.first << " | Human - " << score.second << "\n";
          cout << "New round starting in 3 seconds...";
          selector = make_pair(1, 1);
          sleep(3);
          resetGameBoard();
          break;
        }
      }
    }
    system("CLS");
    drawGameBoard();
  }
}
char checkWinner() {
  // Check by row
  for(int i = 0; i < 3; i++)
    if(gameBoard[i][0] == gameBoard[i][1] && gameBoard[i][1] == gameBoard[i][2] && gameBoard[i][2] != clear)
      return (gameBoard[i][0] == ai) ? xWin : oWin;
  // Check by column
  for(int i = 0; i < 3; i++)
    if(gameBoard[0][i] == gameBoard[1][i] && gameBoard[1][i] == gameBoard[2][i] && gameBoard[2][i] != clear)
      return (gameBoard[0][i] == ai) ? xWin : oWin;
  // Check by diagonals
  if(gameBoard[0][0] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][2] && gameBoard[2][2] != clear)
    return (gameBoard[0][0] == ai) ? xWin : oWin;
  if(gameBoard[2][0] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[0][2] && gameBoard[0][2] != clear)
    return (gameBoard[2][0] == ai) ? xWin : oWin;
  // If is tie
  int clearCells = 0;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      if(gameBoard[i][j] == clear)
        clearCells++;
  if(clearCells == 0)
    return tie;
  // If the game is not over yet
  return nWin;
}
void nextMove() {
  short int bestScore = SHRT_MIN;
  pair<int, int> bestMove;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(gameBoard[i][j] == clear) {
        gameBoard[i][j] = ai;
        short int score = minimax(SHRT_MIN, SHRT_MAX, false);
        gameBoard[i][j] = clear;
        if(score > bestScore) {
          bestScore = score;
          bestMove = make_pair(i, j);
        }
      }
    }
  }
  gameBoard[bestMove.first][bestMove.second] = ai;
}
short int minimax(short int alpha, short int beta, bool isMaximizing) {
  if(checkWinner() == xWin)
    return 1;
  if(checkWinner() == oWin)
    return -1;
  if(checkWinner() == tie)
    return 0;
  bool forceStop = false;
  if(isMaximizing) {
    short int bestEval = SHRT_MIN;
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++) {
        if(gameBoard[i][j] == clear) {
          gameBoard[i][j] = ai;
          short int eval = minimax(alpha, beta, false);
          gameBoard[i][j] = clear;
          bestEval = max(bestEval, eval);
          alpha = max(eval, alpha);
          if(beta <= alpha) {
            forceStop = true;
            break;
          }
        }
      }
      if(forceStop) {
        break;
      }
    }
    return bestEval;
  } else {
    short int bestEval = SHRT_MAX;
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++) {
        if(gameBoard[i][j] == clear) {
          gameBoard[i][j] = human;
          short int eval = minimax(alpha, beta, true);
          gameBoard[i][j] = clear;
          bestEval = min(bestEval, eval);
          beta = min(eval, beta);
          if(beta <= alpha) {
            forceStop = true;
            break;
          }
        }
      }
      if(forceStop) {
        break;
      }
    }
    return bestEval;
  }
}
