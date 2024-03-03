// Copyright 2024 Cameron Dudd

#include "./constants.h"
#include "./math.h"

#include <SDL2/SDL.h>
#include <stdio.h>

int PLAYER_MAX_HEIGHT = (int)WINDOW_HEIGHT - PLAYER_HOR_OFFSET - PLAYER_HEIGHT;

int game_is_running = FALSE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int tmp_int;
int last_frame_time = 0;

struct player player1;
struct player player2;
struct ball ball;

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
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      game_is_running = FALSE;
    } else {
      switch (event.key.keysym.scancode) {
      case 26: // w
        player1.y = int_max(player1.y - PLAYER_SPEED, PLAYER_VER_OFFSET);
        break;
      case 22: // s
        player1.y = int_min(player1.y + PLAYER_SPEED, PLAYER_MAX_HEIGHT);
        break;
      case 82: // up
        player2.y = int_max(player2.y - PLAYER_SPEED, PLAYER_VER_OFFSET);
        break;
      case 81: // down
        player2.y = int_min(player2.y + PLAYER_SPEED, PLAYER_MAX_HEIGHT);
        break;
      case 41: // escape
        game_is_running = FALSE;
        break;
      default:
        fprintf(stderr, "%d\n", event.key.keysym.scancode);
        break;
      }
    }
    break;
  }
}

void init_ball() {
  ball.x = ((float)WINDOW_WIDTH - (float)BALL_WIDTH) / 2;
  ball.y = ((float)WINDOW_HEIGHT - (float)BALL_HEIGHT) / 2;
  ball.width = BALL_WIDTH;
  ball.height = BALL_HEIGHT;
}

void launch_ball() {
  bool should_move_left = rand_val(0, 1) == 1;
  int speed_direction = should_move_left ? -1 : 1;
  ball.speed_x = speed_direction * BALL_INIT_X_SPEED;
  ball.speed_y = BALL_INIT_Y_SPEED;
}

void init_player1() {
  player1.x = PLAYER_HOR_OFFSET;
  player1.y = PLAYER_HOR_OFFSET;
  player1.width = PLAYER_WIDTH;
  player1.height = PLAYER_HEIGHT;
}

void init_player2() {
  player2.x = WINDOW_WIDTH - PLAYER_HOR_OFFSET - PLAYER_WIDTH;
  player2.y = (int)(WINDOW_HEIGHT / 2) - PLAYER_VER_OFFSET - PLAYER_HEIGHT;
  player2.width = PLAYER_WIDTH;
  player2.height = PLAYER_HEIGHT;
}

void init_pos() {
  init_ball();
  init_player1();
  init_player2();
  launch_ball();
}

void setup() { init_pos(); }

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

  if (ball_paddle_collision(&ball, &player1) || // hor collisions with paddles
      ball_paddle_collision(&ball, &player2)) {
    ball.speed_x = -ball.speed_x * 1.02;
  } else if (ball.y <= 0 || // ver collision with edge
             ball.y + BALL_HEIGHT >= WINDOW_HEIGHT) {
    ball.speed_y = -ball.speed_y;
  } else if (ball.x <= 0 || // hor collision with edge
             ball.x + BALL_WIDTH >= WINDOW_WIDTH) {
    init_pos();
  }

  ball.x += ball.speed_x * delta_time;
  ball.y += ball.speed_y * delta_time;
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
