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

int second, millisecond;

// Arduboy Height & Width
const int screenWidth = Arduboy2::width() - 2;
const int screenHeight = Arduboy2::height() - 2;

enum class GameState
{
    Menu,
    Game,
    End
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
    if(second >= 60) gameState = GameState::End;
}

// Timer Integer
void timerInt()
{
    millisecond += 1;
}

void setup()
{

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

        case GameState::End:
            updateEnd();
            drawEnd();
            break;
    }

    arduboy.display();
}

// Menu
void updateMenu()
{
    reset();
    if(arduboy.justPressed(A_BUTTON)) gameState = GameState::Game;
}

void drawMenu()
{
    arduboy.setCursor(31, 23);
    arduboy.print(F("QUICK BLOCK"));

    arduboy.setCursor(28, 33);
    arduboy.print(F("1 Minute Max"));

    arduboy.setCursor(34, 43);
    arduboy.print(F("A to Start"));
}

// Gameplay
void updateGame()
{
    // Player Movement
    if(arduboy.pressed(RIGHT_BUTTON) && playerX < screenWidth) playerX += 1;
    if(arduboy.pressed(LEFT_BUTTON) && playerX >= 10) playerX -= 1;

    if(arduboy.pressed(UP_BUTTON) && playerY > 10) playerY -= 1;
    if(arduboy.pressed(DOWN_BUTTON) && playerY < screenHeight) playerY += 1;

    if(playerX >= screenWidth) playerX -= 1;
    if(playerX <= 10) playerX += 1;

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

    if(score >= 10) gameState = GameState::End;
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

    arduboy.setCursor(68, 0);
    arduboy.print(F("Score:"));
    arduboy.print(score);
    arduboy.print(F("/10"));

    Sprites::drawSelfMasked(playerX, playerY, player, 0);
    Sprites::drawSelfMasked(pointX, pointY, point, 0);
}

int randomisePointX()
{
    size_t indexX = random(10, screenWidth);
    return pointX = indexX;
}

int randomisePointY()
{
    size_t indexY = random(10, screenHeight);
    return pointY = indexY;
}

// Game over
void updateEnd()
{
    FlexiTimer2::stop();
    if(arduboy.justPressed(A_BUTTON)) gameState = GameState::Menu;
}

void drawEnd()
{
    if(second <= 59)
    {
        arduboy.setCursor(40, 22);
        arduboy.print(F("You Win!"));

        arduboy.setCursor(34, 32);
        arduboy.print(F("Time:"));
        arduboy.print(second);
        arduboy.print(F("."));
        arduboy.print(millisecond);
    }
    if(second >= 60)
    {
        arduboy.setCursor(40, 29);
        arduboy.print(F("You Lose"));
    }
}