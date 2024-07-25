#include "game.h"

bool Game::Initialize() {
  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if (sdlResult != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }
  window_ =
      SDL_CreateWindow("Game Programming in C++, Chapter 1", 1024, 768, 0);
  if (!window_) {
    SDL_Log("Failed to create window %s", SDL_GetError());
    return false;
  }
  return true;
}

void Game::ShutDown() {
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void Game::RunLoop() {
  while (is_running_) {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}
