/*
-------------------------------
Names: Meilin Lyu, Ze Hui Peng
IDs: 1577829, 1594884
CMPUT 275 Winter 2019
Final Project: Arduino Maze Runner
-------------------------------
*/

#ifndef _CONSTS_H
#define _CONSTS_H

#include <Arduino.h>

// All arduino pins defined in a namespace
namespace ArduinoPins {
  // the pins used to connect to the AdaFruit display
  const uint8_t sd_cs = 6;
  const uint8_t tft_cs = 10;
  const uint8_t tft_dc = 9;

  // analog pins for joystick axes
  const uint8_t JOY_VERT = A1;
  const uint8_t JOY_HORIZ = A0;

  // joystick button pin
  const uint8_t joy_button_pin = 8;
  const int16_t joy_center = 512;
  const int16_t joy_deadzone = 32;

  // solution button pin
  const uint8_t sol_button_pin = 3;

  // pins for touchscreen
  const uint8_t YM = 5; // y-
  const uint8_t XP = 4; // x+
  const uint8_t YP = A2; // y+
  const uint8_t XM = A3; // x-
}


// All display constants defined in a namespace
namespace displayconsts {
  const int16_t display_width = 320;
  const int16_t display_height = 240;

  const int16_t block_pixel = 8;
  const int16_t cursor_size = 4;

  // constants for touchscreen initialization
  const int16_t TS_MINX  = 150;
  const int16_t TS_MINY = 120;
  const int16_t TS_MAXX = 920;
  const int16_t TS_MAXY = 940;

  // constants for minimum and maximum pressure for touchscreen
  const int16_t MINPRESSURE = 10;
  const int16_t MAXPRESSURE = 1000;
}

// All maze constants defined in a namespace
namespace mazeconsts {
  // size of the maze
  const uint16_t maze_width = 40;
  const uint16_t maze_height = 30;
  const uint16_t maze_size = maze_width * maze_height;

  // button height,width and sizes
  const uint16_t DFS_x1 = 100;
  const uint16_t DFS_y1 = 70;
  const uint16_t Kruskal_x1 = 100;
  const uint16_t Kruskal_y1 = 130;
  const uint16_t Prim_x1 = 100;
  const uint16_t Prim_y1 = 190;
  const uint16_t button_width = 100;
  const uint16_t button_height = 40;
}

#endif 