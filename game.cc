#include "game.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>

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

  renderer_ = SDL_CreateRenderer(window_, NULL);
  if (!renderer_) {
    SDL_Log("Failed to create renderer %s", SDL_GetError());
    return false;
  }
  return true;
}

void Game::GenerateOutput() {
  // set the entire screen to blue and opacity to full 100%
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  // clear the back buffer
  SDL_RenderClear(renderer_);
  // skipping drawing the game for now
  // Finally, swapping the front and back buffers
  SDL_RenderPresent(renderer_);
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

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      is_running_ = false;
      break;
    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_ESCAPE])
    is_running_ = false;
}
