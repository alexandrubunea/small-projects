#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

#define WALL_BORDER         '#'
#define SNAKE_SEGMENT       'o'
#define SNAKE_HEAD          'O'
#define FRUIT_SYMBOL         '*'
#define GAME_TABLE_HEIGHT    10
#define GAME_TABLE_WIDTH     25
#define MAX_SCORE            150

char gameTable[GAME_TABLE_HEIGHT][GAME_TABLE_WIDTH];
bool gameOver = false, hasWon = false;
short int Score = 0;

struct bodySegment {
    short int x;
    short int y;
};
vector<bodySegment> bodyPart;

struct objectFruit {
    short int x;
    short int y;
};
objectFruit fruit;

enum {
    NONE,
    GOING_UP,
    GOING_DOWN,
    GOING_LEFT,
    GOING_RIGHT
};
auto snakeDirection = GOING_UP;

void initDefaultValues();
void drawGameTable();
void readFromKeyboard();
void gameMechanics();

int main() {
    initDefaultValues();
    while(!gameOver) {
        drawGameTable();
        readFromKeyboard();
        gameMechanics();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        system("cls");
    }
    if(hasWon)
      cout << "You won the game! Congrats!";
    else
      cout << "You lose, but don't give up now!";
    return 0;
}

void initDefaultValues() {
    // Draw white spaces
    for(short int i = 0; i < GAME_TABLE_HEIGHT; i++)
        for(short int j = 0; j < GAME_TABLE_WIDTH; j++)
            gameTable[i][j] = ' ';
    // Draw game walls
    for(short int i = 0; i < GAME_TABLE_HEIGHT; i++) {
        gameTable[i][0] = gameTable[i][GAME_TABLE_WIDTH - 1] = WALL_BORDER;
    }
    for(short int i = 0; i < GAME_TABLE_WIDTH; i++) {
        gameTable[0][i] = gameTable[GAME_TABLE_HEIGHT - 1][i] = WALL_BORDER;
    }
    // Snake head & segments
    bodySegment head = {GAME_TABLE_HEIGHT/2, GAME_TABLE_WIDTH/2};
    bodyPart.push_back(head);
    short int fruit_rand_x = (rand() % (GAME_TABLE_HEIGHT -1));
    short int fruit_rand_y = (rand() % (GAME_TABLE_WIDTH - 1));
    if(fruit_rand_x < 0)
      fruit_rand_x = 3;
    if(fruit_rand_y < 0)
      fruit_rand_y = 5;
    fruit.x = fruit_rand_x;
    fruit.y = fruit_rand_y;
}
void drawGameTable() {
    for(short int i = 0; i < GAME_TABLE_HEIGHT; i++) {
        for(short int j = 0; j < GAME_TABLE_WIDTH; j++) {
            bool is_white_space = true;
            for(auto part : bodyPart) {
                if(part.x == i && part.y == j) {
                    is_white_space = false;
                    break;
                }
            }
            if(is_white_space) {
                if(i == fruit.x && j == fruit.y)
                    cout << FRUIT_SYMBOL;
                else
                    cout << gameTable[i][j];
            }
            else {
                if(i == bodyPart[0].x && j == bodyPart[0].y)
                    cout << SNAKE_HEAD;
                else
                    cout << SNAKE_SEGMENT;
            }
        }
        cout << '\n';
    }
    cout << "\n\nScore: " << Score;
}
void readFromKeyboard() {
    if(kbhit()) {
        char keyPressed = getch();
        switch(keyPressed) {
            case 'w': {
                if(snakeDirection != GOING_DOWN)
                  snakeDirection = GOING_UP;
                break;
            }
            case 's': {
                if(snakeDirection != GOING_UP)
                  snakeDirection = GOING_DOWN;
                break;
            }
            case 'a': {
                if(snakeDirection != GOING_RIGHT)
                  snakeDirection = GOING_LEFT;
                break;
            }
            case 'd': {
                if(snakeDirection != GOING_LEFT)
                  snakeDirection = GOING_RIGHT;
                break;
            }
        }
    }
}
void gameMechanics() {
    short int dx[4] = {1,  0, -1,  0},
              dy[4] = {0,  1,  0, -1},
              d = -1;
    switch(snakeDirection) {
        case GOING_DOWN: {
            d = 0; break;
        }
        case GOING_UP: {
            d = 2; break;
        }
        case GOING_LEFT: {
            d = 3; break;
        }
        case GOING_RIGHT: {
            d = 1; break;
        }
    }
    bodySegment oldPart = bodyPart[0];
    bodyPart[0].x += dx[d];
    bodyPart[0].y += dy[d];
    for(short int i = 1; i < bodyPart.size(); i++) {
        bodySegment aux = bodyPart[i];
        bodyPart[i] = oldPart;
        oldPart = aux;
    }
    if(gameTable[bodyPart[0].x][bodyPart[0].y] == WALL_BORDER) {
        gameOver = true;
        return;
    }
    for(short int i = 1; i < bodyPart.size(); i++) {
      if(bodyPart[0].x == bodyPart[i].x && bodyPart[0].y == bodyPart[i].y) {
        gameOver = true;
        return;
      }
    }
    if(bodyPart[0].x == fruit.x && bodyPart[0].y == fruit.y) {
        short int x = bodyPart.back().x, y = bodyPart.back().y,
            new_x = 0, new_y = 0;
        bool hasFoundPosition = false;
        for(short int i = 0; i < 4; i++) {
            new_x = x + dx[i];
            new_y = y + dy[i];
            if(gameTable[new_x][new_y] == ' ') {
                bodySegment newSegment = {new_x, new_y};
                bodyPart.push_back(newSegment);
                hasFoundPosition = true;
                break;
            }
        }
        if(!hasFoundPosition) {
            x++;
            bodySegment newSegment = {x, y};
            bodyPart.push_back(newSegment);
        }
        if(Score > MAX_SCORE) {
            gameOver = true;
            hasWon = true;
        }
        short int fruit_rand_x = (rand() % (GAME_TABLE_HEIGHT -1));
        short int fruit_rand_y = (rand() % (GAME_TABLE_WIDTH - 1));
        if(fruit_rand_x <= 0)
          fruit_rand_x = 3;
        if(fruit_rand_y <= 0)
          fruit_rand_y = 5;
        fruit.x = fruit_rand_x;
        fruit.y = fruit_rand_y;
        Score += 10;
    }
}
