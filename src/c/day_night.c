// -----------------------------------------------------------------------------
// @file c/day_night.c
// -----------------------------------------------------------------------------

#include "cbindgen/bindings.h"

#include <assert.h>
#include <stdbool.h>

struct board_s {
  bool pixels[SIM_X_SIZE][SIM_Y_SIZE];
};

#define ALIVE_COLOR 0xff00ff00u
#define DEAD_COLOR 0xffff0000u

typedef struct board_s board_t;

void randomly_fill_board(board_t *board_ptr) {
  assert(board_ptr);

  for (int x = 0; x < SIM_X_SIZE; ++x) {
    for (int y = 0; y < SIM_Y_SIZE; ++y) {
      board_ptr->pixels[x][y] = ((sim_rand() & 1) == 1) ? true : false;
    }
  }
}

int wrap_coordinate(int x, int offset, int size) {
  assert(offset >= -1 && offset <= 1);
  assert(x >= 0 && x < size);
  if (offset == 0)
    return x;
  if (x == 0 && offset == -1)
    return size - 1;
  if (x == size - 1 && offset == 1)
    return 0;
  return x + offset;
}

bool get_next_cell_state(const board_t *board_ptr, int x_center, int y_center) {
  assert(board_ptr);

  assert(x_center >= 0 && x_center < SIM_X_SIZE);
  assert(y_center >= 0 && y_center < SIM_Y_SIZE);

  int alive_count = 0;
  for (int x_off = -1; x_off <= 1; ++x_off) {
    for (int y_off = -1; y_off <= 1; ++y_off) {
      if (x_off == 0 && y_off == 0)
        continue;
      int x = wrap_coordinate(x_center, x_off, SIM_X_SIZE);
      int y = wrap_coordinate(y_center, y_off, SIM_Y_SIZE);
      alive_count += (board_ptr->pixels[x][y] == true);
    }
  }

  if (board_ptr->pixels[x_center][y_center])
    return (alive_count == 3 || alive_count == 4 || alive_count == 6 ||
            alive_count == 7 || alive_count == 8);
  return (alive_count == 3 || alive_count == 6 || alive_count == 7 ||
          alive_count == 8);
}

void get_next_state(const board_t *prev, board_t *next) {
  assert(prev);
  assert(next);

  for (int x = 0; x < SIM_X_SIZE; ++x) {
    for (int y = 0; y < SIM_Y_SIZE; ++y) {
      next->pixels[x][y] = get_next_cell_state(prev, x, y);
    }
  }
}

void draw_board(const board_t *board_ptr) {
  assert(board_ptr);

  for (int x = 0; x < SIM_X_SIZE; ++x) {
    for (int y = 0; y < SIM_Y_SIZE; ++y) {
      sim_put_pixel(x, y, board_ptr->pixels[x][y] ? ALIVE_COLOR : DEAD_COLOR);
    }
  }
}

#define MAX_STEPS 10000

int main() {
  board_t board1 = {0};
  board_t board2 = {0};
  int board_index = 0;

  sim_init();

  randomly_fill_board(&board1);
  for (int step = 0; step < MAX_STEPS; ++step) {
    if (board_index == 0) {
      draw_board(&board1);
      get_next_state(&board1, &board2);
    } else {
      draw_board(&board2);
      get_next_state(&board2, &board1);
    }

    sim_flush();
    board_index = (board_index + 1) & 1;
  }

  sim_exit();
}
