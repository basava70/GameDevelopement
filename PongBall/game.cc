#include "game.h"
#include "SDL3/SDL_rect.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdlib>
#include <string>

bool Game::Initialize() {
  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if (sdlResult != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  if (TTF_Init() != -1) {
    SDL_Log("Failed to initialize SDL_ttf: %s", SDL_GetError());
    return false;
  }

  // loading the font
  Font_ = TTF_OpenFont("font/FiraCodeNerdFont-Light.ttf", 14);
  if (!Font_) {
    SDL_Log("Failed to load font: %s", SDL_GetError());
    return false;
  }

  Window_ = SDL_CreateWindow("PongBall", WindowWidth_, WindowHeight_, 0);
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

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      IsRunning_ = false;
      break;
    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_ESCAPE])
    IsRunning_ = false;

  PaddleDir_ = 0;

  // move top if W is pressed, negative y direction
  if (state[SDL_SCANCODE_W])
    PaddleDir_ += -1;

  // move bottom if S is pressed, positive y direction
  if (state[SDL_SCANCODE_S])
    PaddleDir_ += 1;
}

void Game::UpdateGame() {
  // Wait until 8.33ms (120 FPS) has elapsed since last frame
  while (!(SDL_GetTicks() - TicksCount_ >= 8)) {
    ;
  }

  /**
   * -- Delta time is the difference in ticks from last frame
   * -- converted to seconds
   **/
  Uint64 currentTicks = SDL_GetTicks();
  float deltaTime = (currentTicks - TicksCount_) / 1000.0f;

  // Clamp maximum delta time to 0.05
  if (deltaTime > 0.05f)
    deltaTime = 0.05f;
  TicksCount_ = SDL_GetTicks();

  // move the paddle accordingly
  // We are moving 300.0f pixels per second
  if (PaddleDir_ != 0) {
    PaddlePos_.y += PaddleDir_ * 600.0f * deltaTime;

    if (PaddlePos_.y < PaddleHeight_ / 2.0f + Thickness_)
      PaddlePos_.y = PaddleHeight_ / 2.0f + Thickness_;

    if (PaddlePos_.y > WindowHeight_ - PaddleHeight_ / 2.0f - Thickness_)
      PaddlePos_.y = WindowHeight_ - PaddleHeight_ / 2.0f - Thickness_;
  }

  // updating the ball
  BallPos_.x += BallVel_.x * deltaTime;
  BallPos_.y += BallVel_.y * deltaTime;

  /**
   * -- Collision detection
   * -- For top wall, if BallPos_.y <= Thickness_, we have a hit in frame A
   * -- Now, we change the direction of ball, BallVel_.y *= -1.
   * -- However, in frame B, we might still be in collision zone, albeit
   * -- in the correct direction, so we must add another condition
   * -- to not get stuck in an infinite collision loop.
   **/

  // top wall collision
  if (BallPos_.y <= Thickness_ * 3.0f / 2 && BallVel_.y < 0.0f)
    BallVel_.y *= -1;

  // right wall collision
  if (BallPos_.x >= WindowWidth_ - Thickness_ * 3.0f / 2 && BallVel_.x > 0.0f)
    BallVel_.x *= -1;

  // bot wall collision
  if (BallPos_.y >= WindowHeight_ - Thickness_ * 3.0f / 2 && BallVel_.y > 0.0f)
    BallVel_.y *= -1;

  // left wall collision **** game ends *****
  if (BallPos_.x <= Thickness_ * 3.0f / 2 && BallVel_.x < 0.0f)
    IsRunning_ = false;

  /**
   * -- Collision with the paddle
   * -- We will check if the abs diff in y values is <= PaddleHeight_/2
   * -- and diff in x values is <= Thickness_
   * -- and BallVel_.x < 0 (ball is going left toward paddle)
   **/
  if (std::abs(BallPos_.y - PaddlePos_.y) <= PaddleHeight_ / 2 &&
      std::abs(BallPos_.x - PaddlePos_.x) <= Thickness_ && BallVel_.x < 0.0f) {
    BallVel_.x *= -1.0f;
    Score_++;
    DifficultyLevel_ += Score_;
    BallVel_.x += DifficultyLevel_;
  }
}

void Game::GenerateOutput() {
  // set the entire screen to black and opacity to full 100%
  SDL_SetRenderDrawColor(Renderer_, 0, 0, 0, 0);
  // clear the back buffer
  SDL_RenderClear(Renderer_);

  // choosing black as the color for paddle and ball
  SDL_SetRenderDrawColor(Renderer_, 255, 255, 255, 255);

  // Creating walls
  const SDL_FRect wall_top{0, 0, WindowWidth_, Thickness_};
  const SDL_FRect wall_bot{0, WindowHeight_ - Thickness_, WindowWidth_,
                           Thickness_};
  const SDL_FRect wall_left{0, 0, Thickness_, WindowHeight_};
  const SDL_FRect wall_right{WindowWidth_ - Thickness_, 0, Thickness_,
                             WindowHeight_};
  SDL_RenderFillRect(Renderer_, &wall_top);
  SDL_RenderFillRect(Renderer_, &wall_bot);
  SDL_RenderFillRect(Renderer_, &wall_left);
  SDL_RenderFillRect(Renderer_, &wall_right);

  /**
   * -- Drawing the paddle
   * -- Choosing green for paddle
   **/
  SDL_SetRenderDrawColor(Renderer_, 0, 255, 0, 255);
  SDL_FRect paddle{PaddlePos_.x - Thickness_ / 2,
                   PaddlePos_.y - PaddleHeight_ / 2, Thickness_, PaddleHeight_};
  SDL_RenderFillRect(Renderer_, &paddle);

  /**
   * -- Drawing ball
   * -- choosing red for ball
   **/
  SDL_SetRenderDrawColor(Renderer_, 255, 0, 0, 255);
  SDL_FRect ball{BallPos_.x - Thickness_ / 2, BallPos_.y - Thickness_ / 2,
                 Thickness_, Thickness_};
  SDL_RenderFillRect(Renderer_, &ball);

  // Drawing the score
  std::string scoreText = "Score:" + std::to_string(Score_);
  // choosing white color for text
  SDL_Color textColor = {255, 255, 255, 255};

  // Create surface and texture for Score:
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(Font_, scoreText.c_str(), textColor);
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(Renderer_, textSurface);

  // Get the width and height of the text
  int textWidth = textSurface->w;
  int textHeight = textSurface->h;

  // Free the surface after creating the texture
  SDL_DestroySurface(textSurface);

  // Define the destination rectangle for the score
  SDL_FRect dstRect;
  // 10 pixels from the right edge
  dstRect.x = WindowWidth_ - textWidth - 10;
  // 10 pixels from the top edge
  dstRect.y = 10;

  dstRect.w = textWidth;
  dstRect.h = textHeight;

  // Render the score texture
  SDL_RenderFillRect(Renderer_, &dstRect);

  // Destroy the texture
  SDL_DestroyTexture(textTexture);

  // Finally, swapping the front and back buffers
  SDL_RenderPresent(Renderer_);
}

void Game::ShutDown() {
  TTF_CloseFont(Font_);
  TTF_Quit();
  SDL_DestroyRenderer(Renderer_);
  SDL_DestroyWindow(Window_);
  SDL_Quit();
}

void Game::RunLoop() {
  while (IsRunning_) {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}
