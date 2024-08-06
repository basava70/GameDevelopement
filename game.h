#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

struct Vector2D {
  float x;
  float y;
  Vector2D(float x_val, float y_val) : x(x_val), y(y_val) {}
};

class Game {
public:
  Game()
      : IsRunning_(true), BallPos_(WindowWidth_ / 2, WindowHeight_ / 2),
        PaddlePos_(Thickness_ * 3 / 2, WindowHeight_ / 2),
        BallVel_(DifficultyLevel_ * -20, DifficultyLevel_ * 21) {}

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
  void UpdateGame();
  void GenerateOutput();

  // Window crated by SDL
  SDL_Window *Window_;
  // bool variable to check if the game is running
  bool IsRunning_;
  SDL_Renderer *Renderer_;

  const float WindowHeight_ = 768, WindowWidth_ = 1024, Thickness_ = 15;
  float DifficultyLevel_ = 10;
  const float PaddleHeight_ = DifficultyLevel_ * Thickness_;

  Uint64 TicksCount_ = 0;

  Vector2D PaddlePos_;
  Vector2D BallPos_;
  Vector2D BallVel_;

  int PaddleDir_ = 0;
  int Score_ = 0;
};
