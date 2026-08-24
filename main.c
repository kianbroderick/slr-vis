#include "stats.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_DIMENSIONS
#define WIDTH 900
#define HEIGHT 600
#define RADIUS 4
#define DASH_SIZE 20
#define SPACE_SIZE 10
#define TOLERANCE 10
#define THICKNESS 3
#define GRID_SIZE 50

int i;
int n = 0;
Vector2 mouse_position;
DataPoint *data;
SLRModel *screen_model;
SLRModel *graph_model;

void draw_grid(int grid_size) {
  for (int x = 0; x < WIDTH; x += grid_size) {
    DrawLine(x, 0, x, HEIGHT, DARKGRAY);
    if (x == WIDTH / 2) {
      DrawLineEx((Vector2){x, 0}, (Vector2){x, HEIGHT}, 5, DARKGRAY);
    }
  }

  for (int y = 0; y < HEIGHT; y += grid_size) {
    DrawLine(0, y, WIDTH, y, DARKGRAY);
    if (y == HEIGHT / 2) {
      DrawLineEx((Vector2){0, y}, (Vector2){WIDTH, y}, 5, DARKGRAY);
    }
  }
}

void draw_line(SLRModel *model) {
  float end_y = model->slope * WIDTH + model->intercept;
  DrawLineEx((Vector2){0, model->intercept}, (Vector2){WIDTH, end_y}, 3, RED);
}

int main() {
  InitWindow(WIDTH, HEIGHT, "SLR");
  SetTargetFPS(60);
  data = NULL;
  screen_model = malloc(sizeof(struct SLRModel));
  graph_model = malloc(sizeof(struct SLRModel));
  while (!WindowShouldClose()) {

    BeginDrawing();
    draw_grid(GRID_SIZE);
    ClearBackground(RAYWHITE);
    DrawGrid(100, 50);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      mouse_position = GetMousePosition();
      data = insert_new_data(data, mouse_position);
      n++;
      printf("Added screen datapoint %d: (%.2f, %.2f)\n", n, mouse_position.x,
             mouse_position.y);
      printf("Added graph datapoint %d: (%.2f, %.2f)\n", n,
             convert_x(mouse_position.x), convert_y(mouse_position.y));
      screen_model = compute_slr(data, screen_model, SCREEN);
      graph_model = compute_slr(data, graph_model, GRAPH);
    }

    if (data != NULL && n > 1) {
      float sst = calculate_sst(graph_model, data, GRAPH);
      float sse = calculate_sse(graph_model, data, GRAPH);
      draw_line(screen_model);
      DrawText(TextFormat("Y = %.2f + %.2fX\n", graph_model->intercept,
                          graph_model->slope),
               10, 10, 30, BLACK);
      DrawText(TextFormat("R^2 = %.2f\n", 1 - (sse / sst)), 10, 60, 30, BLACK);
    }
    for (DataPoint *p = data; p != NULL; p = p->next) {
      DrawCircle(p->screen.x, p->screen.y, RADIUS, BLUE);
    }
    EndDrawing();
  }
  CloseWindow();
}
