#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
class Game {
public:
  Game() : is_running_(true) {}

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
  void ProcessInput() {}
  void UpdateGame() {}
  void GenerateOutput() {}

  // Window crated by SDL
  SDL_Window *window_;
  // bool variable to check if the game is running
  bool is_running_;
};
