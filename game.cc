#include "game.h"
#include "SDL3/SDL_rect.h"
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
  Window_ =
      SDL_CreateWindow("Game Programming in C++, Chapter 1", 1024, 768, 0);
  if (!Window_) {
    SDL_Log("Failed to create window %s", SDL_GetError());
    return false;
  }

  Renderer_ = SDL_CreateRenderer(Window_, NULL);
  if (!Renderer_) {
    SDL_Log("Failed to create renderer %s", SDL_GetError());
    return false;
  }
  return true;
}

void Game::GenerateOutput() {
  // set the entire screen to blue and opacity to full 100%
  SDL_SetRenderDrawColor(Renderer_, 0, 0, 255, 255);
  // clear the back buffer
  SDL_RenderClear(Renderer_);

  // choosing black as the color for paddle and ball
  SDL_SetRenderDrawColor(Renderer_, 255, 255, 255, 255);

  // Creating walls
  const int thickness = 15;
  const SDL_FRect wall_top{0, 0, 1024, thickness};
  const SDL_FRect wall_bot{0, 768 - thickness, 1024, thickness};
  const SDL_FRect wall_left{0, 0, thickness, 768};
  const SDL_FRect wall_right{1024 - thickness, 0, thickness, 768};
  SDL_RenderFillRect(Renderer_, &wall_top);
  SDL_RenderFillRect(Renderer_, &wall_bot);
  SDL_RenderFillRect(Renderer_, &wall_left);
  SDL_RenderFillRect(Renderer_, &wall_right);
  // skipping drawing the game for now
  // Finally, swapping the front and back buffers
  SDL_RenderPresent(Renderer_);
}

void Game::ShutDown() {
  SDL_DestroyRenderer(Renderer_);
  SDL_DestroyWindow(Window_);
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
