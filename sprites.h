#pragma once
#include <stdint.h>

constexpr uint8_t playerWidth = 2;
constexpr uint8_t playerHeight = 2;

const unsigned char player[] PROGMEM = {
  // Dimensions
  playerWidth, playerHeight,
  
  0x03, 0x03
};

#pragma once
#include <stdint.h>

constexpr uint8_t pointWidth = 2;
constexpr uint8_t pointHeight = 2;

const unsigned char point[] PROGMEM = {
  // Dimensions
  pointWidth, pointHeight,
  
  0x03, 0x03
};