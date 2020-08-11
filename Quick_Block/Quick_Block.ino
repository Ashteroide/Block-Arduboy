// Block the Game, Achieve the fastest time
// Ashlee J
// Created 10, August, 2020

#include <Arduboy2.h>
#include <FlexiTimer2.h>

#include "sprites.h"

Arduboy2 arduboy;

// Player
int playerX, playerY;

// Dot for points
int pointX, pointY;

// Score and Time
int score;
int maxTime[4] = { 15, 30, 60, 120 };
int maxTimeSelect;

int maxScore[4] = { 5, 10, 15, 20 };
int maxScoreSelect;

int second, millisecond;

// Menu Cursor
int menuCursor;

// Arduboy Height & Width
const int screenWidth = Arduboy2::width() - 2;
const int screenHeight = Arduboy2::height() - 2;

enum class GameState
{
    Menu,
    Game,
    gameOver,
};

GameState gameState = GameState::Menu;

// Reset
void reset()
{
    randomisePointX();
    randomisePointY();

    playerX = (Arduboy2::width() / 2) - 1;
    playerY = (Arduboy2::height() / 2) - 1;

    score = 0;
    millisecond = 0;
    second = 0;

    FlexiTimer2::set(10, timerInt);
    FlexiTimer2::start();
}

// Timer
void Timer()
{
    if(millisecond >= 100)
    {
        second += 1;
        millisecond = 0;
    }
}

// Timer Integer
void timerInt()
{
    millisecond += 1;
}

void setup()
{
    maxScoreSelect = 0;
    maxTimeSelect = 0;
    menuCursor = 33;

    reset();
    arduboy.begin();
    arduboy.clear();
}

void loop()
{
    arduboy.initRandomSeed();

    Timer();
    if(!arduboy.nextFrame()) return;

    arduboy.pollButtons();
    arduboy.clear();

    switch(gameState)
    {
        case GameState::Menu:
            updateMenu();
            drawMenu();
            break;
        
        case GameState::Game:
            updateGame();
            drawGame();
            break;

        case GameState::gameOver:
            updateGameOver();
            drawGameOver();
            break;
    }

    arduboy.display();
}

// Menu
void updateMenu()
{
    reset();
    if(arduboy.justPressed(A_BUTTON)) gameState = GameState::Game;

    if(arduboy.justPressed(UP_BUTTON) && menuCursor == 33) menuCursor = 23;
    if(arduboy.justPressed(DOWN_BUTTON) && menuCursor == 23) menuCursor = 33;

    if(arduboy.justPressed(B_BUTTON) && menuCursor == 33)
    {
        maxScoreSelect += 1;
        if(maxScoreSelect >= 4) maxScoreSelect = 0;
    }
    if(arduboy.justPressed(B_BUTTON) && menuCursor == 23)
    {
        maxTimeSelect += 1;
        if(maxTimeSelect >= 4) maxTimeSelect = 0;
    }
}

void drawMenu()
{
    arduboy.setCursor(31, 13);
    arduboy.print(F("QUICK BLOCK"));

    if(menuCursor == 33) arduboy.setCursor(28, menuCursor);
    else if(menuCursor == 23) arduboy.setCursor(18, menuCursor);
    arduboy.print(F("B>"));

    arduboy.setCursor(31, 23);
    arduboy.print(F("Time:"));
    arduboy.print(maxTime[maxTimeSelect]);
    arduboy.print(F("secs"));

    arduboy.setCursor(42, 33);
    arduboy.print(F("Score:"));
    arduboy.print(maxScore[maxScoreSelect]);

    arduboy.setCursor(34, 43);
    arduboy.print(F("A to Start"));
}

// Gameplay
void updateGame()
{
    // Player Movement
    if(arduboy.pressed(RIGHT_BUTTON) && playerX < screenWidth - 2) playerX += 1;
    if(arduboy.pressed(LEFT_BUTTON) && playerX >= 2) playerX -= 1;

    if(arduboy.pressed(UP_BUTTON) && playerY > 23) playerY -= 1;
    if(arduboy.pressed(DOWN_BUTTON) && playerY < screenHeight - 2) playerY += 1;

    if(playerX >= screenWidth - 2) playerX -= 1;
    if(playerX <= 2) playerX += 1;

    // Point Position

    if(pointXNearPlayerX())
    {
        if(pointYNearPlayerY())
        {
            randomisePointX();
            randomisePointY();

            score += 1;
        }
    }

    if(score >= maxScore[maxScoreSelect]) gameState = GameState::gameOver;

    if(second >= maxTime[maxTimeSelect]) gameState = GameState::gameOver;
}

bool pointXNearPlayerX()
{
    return(playerX == pointX || (playerX == pointX + 2 || playerX == pointX - 2) || (playerX == pointX + 1 || playerX == pointX - 1));
}

bool pointYNearPlayerY()
{
    return(playerY == pointY || (playerY == pointY + 2 || playerY == pointY - 2) || (playerY == pointY + 1 || playerY == pointY - 1));
}

void drawGame()
{
    arduboy.setCursor(0, 0);
    arduboy.print(F("Time:"));
    arduboy.print(second);
    arduboy.print(F("."));
    arduboy.print(millisecond);

    arduboy.setCursor(0, 10);
    arduboy.print(F("Score:"));
    arduboy.print(score);
    arduboy.print(F("/"));
    arduboy.print(maxScore[maxScoreSelect]);

    Sprites::drawSelfMasked(playerX, playerY, player, 0);
    Sprites::drawSelfMasked(pointX, pointY, point, 0);
    Sprites::drawSelfMasked(0, 20, border, 0);
}

int randomisePointX()
{
    size_t indexX = random(23, screenWidth - 2);
    return pointX = indexX;
}

int randomisePointY()
{
    size_t indexY = random(23, screenHeight - 2);
    return pointY = indexY;
}

// Game over
void updateGameOver()
{
    if(arduboy.justPressed(A_BUTTON)) gameState = GameState::Menu;
    FlexiTimer2::stop();
}

void drawGameOver()
{
    if(second <= maxTime[maxTimeSelect] - 1)
    {
        arduboy.setCursor(40, 22);
        arduboy.print(F("You Win!"));

        if(second < 10) arduboy.setCursor(36, 32);
        else arduboy.setCursor(34, 32);
        arduboy.print(F("Time:"));
        arduboy.print(second);
        arduboy.print(F("."));
        arduboy.print(millisecond);
    }

    if(second >= maxTime[maxTimeSelect])
    {
        arduboy.setCursor(40, 29);
        arduboy.print(F("You Lose!"));
    }
}