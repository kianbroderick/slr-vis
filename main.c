#include "plotting.h"
#include "slr.h"
#include "time.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RADIUS 4
#define DASH_SIZE 20
#define SPACE_SIZE 10
#define TOLERANCE 10
#define THICKNESS 3
#define K 5
#define W 0.70

int i;
Vector2 mouse_position;
Data data = {.mode = SLR, .n = 0, .head = NULL};
Data *pdata = &data;
int grid_size = GRID_SIZE;

int main() {
  if ((HEIGHT % GRID_SIZE != 0) || (WIDTH % GRID_SIZE != 0)) {
    puts("ERROR: Window dimensions not a multiple of grid size.");
    return 1;
  }
  InitWindow(WIDTH, HEIGHT, "SLR");
  SetTargetFPS(60);
  pdata->head = NULL;
  while (!WindowShouldClose()) {
    mouse_position = GetMousePosition();
    if (IsKeyPressed(KEY_R)) {
      reset(pdata);
      puts("Reset data points.");
    }
    if (IsKeyPressed(KEY_ONE)) {
      reset(pdata);
      pdata->mode = SLR;
      puts("SLR mode.");
    }
    if (IsKeyPressed(KEY_TWO)) {
      if (pdata->mode == SLR)
        reset(pdata);
      pdata->mode = AVERAGE_SMOOTHING;
      puts("Moving average smoothing mode.");
    }
    if (IsKeyPressed(KEY_THREE)) {
      if (pdata->mode == SLR)
        reset(pdata);
      pdata->mode = EXPONENTIAL_SMOOTHING;
      puts("Exponential smoothing mode.");
    }
    if (IsKeyPressed(KEY_A)) {
      reset(pdata);
      grid_size = update_grid_size(grid_size, false, WIDTH, HEIGHT);
      printf("new smaller grid size = %d\n", grid_size);
    }
    if (IsKeyPressed(KEY_S)) {
      reset(pdata);
      grid_size = update_grid_size(grid_size, true, WIDTH, HEIGHT);
      printf("new larger grid size = %d\n", grid_size);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      if (pdata->mode == SLR) {
        insert_new_data(pdata, mouse_position, WIDTH, HEIGHT, grid_size);
        printf("Added screen datapoint %d: (%.2f, %.2f)\n", pdata->n,
               mouse_position.x, mouse_position.y);
        printf("Added graph datapoint %d: (%.2f, %.2f)\n", pdata->n,
               convert_x(mouse_position.x, WIDTH, grid_size),
               convert_y(mouse_position.y, HEIGHT, grid_size));
      } else if (pdata->mode == AVERAGE_SMOOTHING) {
        if (pdata->n < WIDTH / grid_size) {
          insert_new_data(pdata,
                          (Vector2){pdata->n * grid_size, mouse_position.y},
                          WIDTH, HEIGHT, grid_size);
          printf("Added graph datapoint %d: (%.2f, %.2f)\n", pdata->n,
                 convert_x(pdata->n * grid_size, WIDTH, grid_size),
                 convert_y(mouse_position.y, HEIGHT, grid_size));
        } else {
          puts("Max datapoints reached. Cannot add more without resetting.");
        }
      } else if (pdata->mode == EXPONENTIAL_SMOOTHING) {
        if (pdata->n < WIDTH / grid_size) {
          insert_new_data(pdata,
                          (Vector2){pdata->n * grid_size, mouse_position.y},
                          WIDTH, HEIGHT, grid_size);
          printf("Added graph datapoint %d: (%.2f, %.2f)\n", pdata->n,
                 convert_x(pdata->n * grid_size, WIDTH, grid_size),
                 convert_y(mouse_position.y, HEIGHT, grid_size));
        } else {
          puts("Max datapoints reached. Cannot add more without resetting.");
        }
      }
    }
    if (pdata->mode == AVERAGE_SMOOTHING) {
      smooth_moving_average(pdata->head, K);
    } else if (pdata->mode == EXPONENTIAL_SMOOTHING) {
      smoothing_exponential(pdata->head, W);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_grid(grid_size);
    draw_points(pdata->head, RADIUS, BLUE);
    if (pdata->mode != SLR) {
      DrawCircle(pdata->n * grid_size, mouse_position.y, RADIUS, RED);
    }

    if (pdata->mode == SLR) {
      draw_model_text(pdata);
    } else if (pdata->mode == AVERAGE_SMOOTHING) {
      draw_avg_smooth_predictions(pdata->head, K, RADIUS, GREEN);
    } else if (pdata->mode == EXPONENTIAL_SMOOTHING) {
      draw_exp_smooth_predictions(pdata->head, RADIUS, GREEN);
    }
    EndDrawing();
  }
  CloseWindow();
}
