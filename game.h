#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>

struct Vector2D {
  int x;
  int y;
  Vector2D(int x_val, int y_val) : x(x_val), y(y_val) {}
};

class Game {
public:
  Game() : is_running_(true), BallPos_(512, 357), PaddlePos_(0, 357) {}

  // initialize the game
  bool Initialize();

  // runs the game loop, updates and takes keystrokes
  void RunLoop();

  // quits the game
  void ShutDown();

private:
  /**
   * -- part of the running loop
   * -- first, we take input from keyboard/joystick/mouse
   * -- second, we update the game
   * -- third, we generate the output of the game
   */
  void ProcessInput();
  void UpdateGame() {}
  void GenerateOutput();

  // Window crated by SDL
  SDL_Window *Window_;
  // bool variable to check if the game is running
  bool is_running_;
  SDL_Renderer *Renderer_;

  Vector2D PaddlePos_;
  Vector2D BallPos_;
};
