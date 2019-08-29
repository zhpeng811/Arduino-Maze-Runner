/*
-------------------------------
Names: Meilin Lyu, Ze Hui Peng
IDs: 1577829, 1594884
CMPUT 275 Winter 2019
Final Project: Arduino Maze Runner
-------------------------------
*/

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include "consts.h"

using namespace ArduinoPins;
using namespace displayconsts;
using namespace mazeconsts;

Adafruit_ILI9341 tft = Adafruit_ILI9341(tft_cs, tft_dc);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint touch;

int cursorX,cursorY;
int X_idx, Y_idx; // maze indexes mapped by cursorX and cursorY
int maze_map[maze_height][maze_width];
int sol_path[maze_size / 4]; //maximum number of nodes is maze_size / 4

// setup function to initialize the Arduino
void setup() {
  init();
  Serial.begin(9600);

  tft.begin();
  pinMode(sol_button_pin, INPUT_PULLUP);

  tft.setRotation(3);
  Serial.flush();
}


/* Description: Draw the cursor to the new position and
                erase the cursor at the old position
                This function is from weekly exercise 1 solution
   Args:  int oldX: old cursor x position
          int oldY: old cursor y position
          int newX: new cursor x position
          int newY: new cursor y position
*/
void draw_cursor(int oldX, int oldY, int newX, int newY) {
  tft.fillRect(oldX - cursor_size/2, oldY - cursor_size/2,
               cursor_size, cursor_size, ILI9341_BLACK);
  tft.fillRect(newX - cursor_size/2, newY - cursor_size/2,
               cursor_size, cursor_size, ILI9341_RED);
}


/* Description: check if any four corners of the cursor
                touches a wall
   returns: true if at least one of the corners hit a wall
            false if no corners hit a wall
*/
bool check_wall() {
  // check if top left corner of the cursor hit a wall
  X_idx = (cursorX - (cursor_size / 2) ) / block_pixel;
  Y_idx = (cursorY - (cursor_size / 2) ) / block_pixel;
  if (maze_map[Y_idx][X_idx] == 0) {
    return true;
  }
  // check if top right corner of the cursor hit a wall
  X_idx = (cursorX + (cursor_size / 2) - 1) / block_pixel;
  Y_idx = (cursorY - (cursor_size / 2) ) / block_pixel;
  if (maze_map[Y_idx][X_idx] == 0) {
    return true;
  } 
  // check if bottom left corner of the cursor hit a wall
  X_idx = (cursorX - (cursor_size / 2) ) / block_pixel;
  Y_idx = (cursorY + (cursor_size / 2) - 1) / block_pixel;
  if (maze_map[Y_idx][X_idx] == 0) {
    return true;
  } 

  // check if bottom right corner of the cursor hit a wall
  X_idx = (cursorX + (cursor_size / 2) - 1) / block_pixel;
  Y_idx = (cursorY + (cursor_size / 2) - 1) / block_pixel;
  if (maze_map[Y_idx][X_idx] == 0) {
    return true;
  } 

  return false;
}


/* Description: move the cursor when the user moves the joystick
                also constrain the cursor within the display range
*/
void move_cursor(){
  int xVal = analogRead(JOY_HORIZ);
  int yVal = analogRead(JOY_VERT);
  int oldX = cursorX;
  int oldY = cursorY;

  // a small flaw is that we can't support custom movement speed
  // because that will conflict with our implementation on checking
  // if the cursor hit a wall
  if (yVal < joy_center - joy_deadzone) { // move up
    cursorY -= 1;
  } 
  else if (yVal > joy_center + joy_deadzone) { //move down
    cursorY += 1; 
  }
  if (xVal > joy_center + joy_deadzone) { // move left
    cursorX -= 1;
  } 
  else if (xVal < joy_center - joy_deadzone) { // move right
    cursorX += 1;
  }

  // terminate the function if the position did not change
  if (cursorX == oldX && cursorY == oldY) {
    return;
  }

  // contrain the cursor within the display range
  cursorX = constrain(cursorX, cursor_size / 2, display_width - cursor_size/2);
  cursorY = constrain(cursorY, cursor_size / 2, display_height - cursor_size/2);

  // check if the cursor hits a wall
  bool wall = check_wall();
  if (wall) {
    cursorX = oldX;
    cursorY = oldY;
    return;
  }

  if ( (oldX != cursorX or oldY != cursorY)) {
    // if any of the corners did not hit a wall
    draw_cursor(oldX, oldY, cursorX, cursorY);
  }

}


/* Description: draw the wall to the display screen after
                the maze map is recieved from the server
*/
void draw_wall() {
    tft.fillScreen(ILI9341_BLACK);
    for (uint16_t i = 0; i < maze_height; i++) {
      // i represents the row of the maze
      for (uint16_t j = 0; j < maze_width; j++) {
        // j represents the colomn of the maze
        if (maze_map[i][j] == 0) {
          // "0" is used to represent the wall
          tft.fillRect(j*block_pixel,i*block_pixel,
                       block_pixel, block_pixel, ILI9341_WHITE);
        }
      }
    }

    // initialize and draw the cursor to the top left of the screen
    cursorX = cursor_size/2;
    cursorY = cursor_size/2;
    draw_cursor(cursorX,cursorY,cursorX,cursorY);

    // mark the end point as a green square
    tft.fillRect(304,224,block_pixel,block_pixel, ILI9341_GREEN);
}


// Description: Print welcome information to the screen
void start_screen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(80,40);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_RED);
  tft.println("Welcome to");
  tft.setCursor(40,80);
  tft.setTextColor(ILI9341_BLUE);
  tft.println("Arduino Maze Runner");
  tft.setCursor(0,160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Press anywhere to continue");

  while (true) {
    touch = ts.getPoint();

    if (touch.z > MINPRESSURE and touch.z < MAXPRESSURE) {
      return;
    }
  }
}


// Description: Draw buttons that prompt the user to select an algorithm
void choose_algorithm(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 40);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Please select an algorithm");
  
  tft.fillRect(DFS_x1, DFS_y1, button_width, button_height, ILI9341_ORANGE);
  tft.setCursor(DFS_x1 + 30,DFS_y1 + 15);
  tft.setTextColor(ILI9341_BLACK);
  tft.println("DFS");
  tft.fillRect(Kruskal_x1, Kruskal_y1, button_width, button_height, ILI9341_GREEN);
  tft.setCursor(Kruskal_x1 + 10, Kruskal_y1 + 15);
  tft.setTextColor(ILI9341_BLACK);
  tft.println("Kruskal");
  tft.fillRect(Prim_x1, Prim_y1, button_width, button_height, ILI9341_BLUE);
  tft.setCursor(Prim_x1 + 25, Prim_y1 + 15);
  tft.println("Prim");
}


/* Description: detects if the user selects one of the algorithms
   Returns: int algorithm: -1 if the user touches an invalid area
                           or the touch pressure is too light or too hard
                           0 for DFS algorithm
                           1 for Kruskal's Minimum Spanning Tree
                           2 for Prim's Minimum Spanning Tree
*/
int detect_touch() {
  touch = ts.getPoint();

  if (touch.z < MINPRESSURE or touch.z > MAXPRESSURE) {
    // check if the screen have been touched within the pressure range
    return -1;

  }

  // x and y are flipped due to the rotation of the screen
  int16_t screen_x = map(touch.y, TS_MINY, TS_MAXY, display_width-1, 0);
  int16_t screen_y = map(touch.x, TS_MINX, TS_MAXX, 0, display_height-1);

  if (screen_x > DFS_x1 && screen_x < DFS_x1 + button_width && 
      screen_y > DFS_y1 && screen_y < DFS_y1 + button_height) {
    return 0;
  }
  else if(screen_x > Kruskal_x1 && screen_x < Kruskal_x1 + button_width && 
          screen_y > Kruskal_y1 && screen_y < Kruskal_y1 + button_height) {
    return 1;
  }
  else if(screen_x > Prim_x1 && screen_x < Prim_x1 + button_width &&
          screen_y > Prim_y1 && screen_y < Prim_y1 + button_height) {
    return 2;
  }

  return -1;
}


/* Description: Display the waiting message while the server is 
                generating the maze.
   int algorithm: algorithm used to generate the maze
*/
void wait_screen(int algorithm) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(30,40);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Generating Maze using");
  tft.setTextColor(ILI9341_RED);

  // print the algorithms that is used
  // and also send the request to server
  if (algorithm == 0) {
    Serial.println('D');
    tft.setCursor(60,80);
    tft.println("DepthFirstSearch");
  }
  else if(algorithm == 1) {
    Serial.println('K');
    tft.setCursor(65,80);
    tft.println("Kruskal's MST");
  }
  else if (algorithm == 2) {
    Serial.println('P');
    tft.setCursor(75,80);
    tft.println("Prim's MST");
  }

  tft.setCursor(60,160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Please wait...");
}


/* Description: Display the end message after the user
                finishes the game
   int end_time: the time used to finish the game (in milliseconds)
*/
void end_message(unsigned long end_time) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(50,40);
  tft.setTextColor(ILI9341_RED);
  tft.println("Escape Successful!");

  tft.setCursor(0, 100);
  tft.setTextColor(ILI9341_ORANGE);
  tft.println("Time Used: ");
  tft.setCursor(30, 120);
  end_time = end_time / 1000; // convert it to seconds
  tft.setTextColor(ILI9341_WHITE);

  tft.print(end_time/60); // minutes
  tft.print(" Min(s) ");
  tft.print(end_time%60); // seconds
  tft.print(" Second(s)");

  tft.setCursor(0,160);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Press anywhere to continue");
  while (true) {
    touch = ts.getPoint();

    if (touch.z > MINPRESSURE and touch.z < MAXPRESSURE) {
      return;
    }
  }

}


/* Description: get the solution path from the server and store
                it in sol_path[] array.
   Returns: int numwaypoints: the number of waypoints from start
                              point to end point.
   Note: if timeout occurs during any part of the communication,
         it will return 0 as the number of waypoints indicating timeout.
         timeout of 3 seconds is used to receive number of waypoints
         timeout of 1 second is used to receive each vertex for the path
*/
int get_solution() {
  int numwaypoints, vertex;
  // recieve the number of waypoints
  unsigned long timeout = millis() + 3000;
  while (true) {
    if (Serial.available() > 0) {
      numwaypoints = Serial.readStringUntil('\n').toInt();
      Serial.println('A');
      break;
    }
    else if (timeout < millis()) {
      return 0;
    }
  }

  // recieve the vertex information for each waypoint
  for (int i = 0; i < numwaypoints; i++) {
    timeout = millis() + 1000;
    while (true) {
      if (Serial.available() > 0) {
        vertex = Serial.readStringUntil('\n').toInt();
        sol_path[i] = vertex;
        Serial.println('A');
        break;
      }
      else if(timeout < millis()) {
        return 0;
      }
    }
  }

  return numwaypoints;
}


/* Description: draw the solution path to the display screen
   Args: int numwaypoints: the number of waypoints from start
                           point to end point.
*/ 
void draw_solution(int numwaypoints) {
  int vertex, nbr, x1, y1, x2, y2;

  // the procedure is similar to the assignment 2 where
  // we have to draw the driving route
  for (int i = 0; i < numwaypoints - 1; i++) {
    vertex = sol_path[i];
    nbr = sol_path[i+1];

    // calculations of the vertex on the solution path
    x1 = (vertex % maze_width) * block_pixel + block_pixel/2;
    y1 = (vertex / maze_width) * 2 * block_pixel + block_pixel/2;
    x2 = (nbr % maze_width) * block_pixel + block_pixel/2;
    y2 = (nbr / maze_width) * 2 * block_pixel + block_pixel/2;

    tft.drawLine(x1, y1, x2, y2, ILI9341_GREEN);
  }
}


/* Description: Process the String read from the read_map() function
                each String represents one row of the maze map
   Args: String line: line read from read_map()
         int height_idx: the current height(row) index to process
*/
void process_String(String line, int height_idx) {
  int width_idx = 0;
  // by convention between the server and client
  // only even indices contain map information
  // odd indices will always be an whitespace
  for(uint16_t i = 0; i < line.length(); i += 2) {
    if (line.substring(i,i+1) == "1") {
      maze_map[height_idx][width_idx] = 1; // path
    }
    else {
      maze_map[height_idx][width_idx] = 0; // wall
    }
    width_idx++;
  }
}


/* Description: read a row of the map from the server
                and send 'A' when the information is received
*/
bool read_map() {
  String line;
  int height_idx = 0;
  unsigned long timeout;
  while (height_idx < maze_height) {
    timeout = millis() + 1000;

    while (true) {
      if (Serial.available() > 0) {
        line = Serial.readStringUntil('\n');
        process_String(line, height_idx);
        height_idx++;
        Serial.println('A');
        break;
      }
      else if (timeout < millis()) {
        return true;
      }
    }

  }

  return false;
}


// Description: display the timeout message to the user
void timeout_message() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(100,100);
  tft.setTextColor(ILI9341_RED);
  tft.println("TIMEOUT!");
  delay(3000);
}


/* Description: client communication part, this part also
                check if timeout occurs when recieving maze
                map information
   Returns: true if timeout occurs
            false otherwise
*/
bool client() {
  bool timeout = false;
  timeout = read_map();
  if (timeout) {
    return true;
  }
  draw_wall();

  return false;
}


/* Description: This function processes ingame cursor movements
                and count the time the user takes to run through the maze. 
                If the user ask for a solution, it will also send an request
                to the server and check if timeout occurs when communicating
                with the server.
*/
void ingame() {
  unsigned long start_time = millis();
  int numwaypoints;

  while (true) {
    move_cursor();

    // if the user want solution for the maze
    if (digitalRead(sol_button_pin) == LOW) {
      Serial.println('S');
      numwaypoints = get_solution();

      // terminate the function if timeout occured when
      // the client trying to get solution from server
      if (numwaypoints == 0) {
        timeout_message();
        return;
      }

      draw_solution(numwaypoints);
    }

    // if the cursor reaches the bottom right corner of the screen
    if (cursorX >= 306 && cursorY >= 226) {
      unsigned long end_time = millis() - start_time;
      end_message(end_time);
      return;
    }

    delay(20); // prevent the cursor from moving too fast
  }
}


int main() {
  setup();

  /* algorithm 0 = DFS, algorithm 1 = Kruskal's MST, 
  algorithm 2 = Prim's MST */
  int algorithm;
  bool timeout;

  while (true) {
    algorithm = -1;
    start_screen();
    choose_algorithm();
    while (algorithm == -1) {
      algorithm = detect_touch();
    }
    wait_screen(algorithm);

    // if timeout occurs during communication, the game will bring
    // the user back to the start screen
    timeout = client();
    if (timeout) {
      timeout_message();
      continue;
    }

    ingame();
  }

  Serial.end();

  return 0;
}