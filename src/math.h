// Copyright 2024 Cameron Dudd
#include <stdbool.h>

struct player {
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
};

extern int int_min(int a, int b);
extern int int_max(int a, int b);
extern int rand_val(int lower, int upper);

extern bool ball_paddle_collision(struct ball *ball, struct player *paddle);
