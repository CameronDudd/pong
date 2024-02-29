// Copyright [2024] Cameron Dudd

#include "./constants.h"

#include <SDL2/SDL.h>
#include <stdio.h>

int game_is_running = FALSE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int last_frame_time = 0;

struct paddle {
  float x;
  float y;
  float width;
  float height;
};

struct ball {
  float x;
  float y;
  float speed_x;
  float speed_y;
  float width;
  float height;
} ball;

struct paddle player1;
struct paddle player2;

int initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return FALSE;
  }
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return FALSE;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return FALSE;
  }
  return TRUE;
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    game_is_running = FALSE;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      game_is_running = FALSE;
    if (event.key.keysym.sym == SDLK_UP)
      player2.y -= 20;
    if (event.key.keysym.sym == SDLK_DOWN)
      player2.y += 20;
    if (event.key.keysym.sym == SDLK_LEFT)
      player2.x -= 20;
    if (event.key.keysym.sym == SDLK_RIGHT)
      player2.x += 20;
    break;
  case SDL_KEYUP:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      game_is_running = FALSE;
    if (event.key.keysym.sym == SDLK_UP)
      ball.speed_y = 0;
    if (event.key.keysym.sym == SDLK_DOWN)
      ball.speed_y = 0;
    if (event.key.keysym.sym == SDLK_LEFT)
      ball.speed_x = 0;
    if (event.key.keysym.sym == SDLK_RIGHT)
      ball.speed_x = 0;
    break;
  }
}

void setup() {
  ball.x = 20;
  ball.y = 20;
  ball.speed_x = 0;
  ball.speed_y = 0;
  ball.width = BALL_WIDTH;
  ball.height = BALL_HEIGHT;
  player1.x = PLAYER_HOR_OFFSET;
  player1.y = PLAYER_HOR_OFFSET;
  player1.width = PLAYER_WIDTH;
  player1.height = PLAYER_HEIGHT;
  player2.x = WINDOW_WIDTH - PLAYER_HOR_OFFSET - PLAYER_WIDTH;
  player2.y = (int)(WINDOW_HEIGHT / 2) - PLAYER_HOR_OFFSET - PLAYER_HEIGHT;
  player2.width = PLAYER_WIDTH;
  player2.height = PLAYER_HEIGHT;
}

void update() {

  // Sleep the execution until we reach the target frame time in milliseconds
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

  // Only call delay if we are too fast to process this frame
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  // Get a delta time factor converted to seconds to be used to update my
  // objects
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  last_frame_time = SDL_GetTicks();

  ball.x += ball.speed_x * delta_time;
  ball.y += ball.speed_y * delta_time;

  fprintf(stdout, "Player 1: %f, %f Player 2: %f %f\n", player1.x, player1.y,
          player2.x, player2.y);
}

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw Ball
  SDL_Rect ball_rect = {(int)ball.x, (int)ball.y, (int)ball.width,
                        (int)ball.height};
  // Draw Player 1
  SDL_Rect player1_rect = {(int)player1.x, (int)player1.y, (int)player1.width,
                           (int)player1.height};
  // Draw Player 2
  SDL_Rect player2_rect = {(int)player2.x, (int)player2.y, (int)player2.width,
                           (int)player2.height};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &ball_rect);
  SDL_RenderFillRect(renderer, &player1_rect);
  SDL_RenderFillRect(renderer, &player2_rect);

  SDL_RenderPresent(renderer);
}

void destroy_window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char *args[]) {
  game_is_running = initialize_window();

  setup();

  while (game_is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();

  return 0;
}
