// Copyright 2024 Cameron Dudd
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./constants.h"
#include "./math.h"

int int_min(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}

int int_max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

int rand_val(int lower, int upper) {
  return rand() % (upper - lower + 1) + lower;
}

bool ball_paddle_collision(struct ball *ball, struct player *paddle) {
  // paddle vals
  int paddle_min_x = paddle->x;
  int paddle_max_x = paddle->x + PLAYER_WIDTH;
  int paddle_min_y = paddle->y;
  int paddle_max_y = paddle->y + PLAYER_HEIGHT;

  // ball vals
  int ball_min_x = ball->x;
  int ball_max_x = ball->x + BALL_WIDTH;
  int ball_min_y = ball->y;
  int ball_max_y = ball->y + BALL_HEIGHT;

  return paddle_min_x <= ball_max_x && ball_min_x <= paddle_max_x &&
         paddle_min_y <= ball_max_y && ball_min_y <= paddle_max_y;
}
