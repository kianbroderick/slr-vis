#include "plotting.h"
#include "raylib.h"
#include <stdlib.h>

float convert_x(float x, int width, int grid_size) {
  return (x - (width / 2)) / grid_size;
}
float convert_y(float y, int height, int grid_size) {
  return ((height / 2) - y) / grid_size;
}

int update_grid_size(int current_size, bool increasing, int width, int height) {
  int inc = increasing ? 1 : -1;
  int new_size = current_size + inc;
  int max = width < height ? width : height;

  while ((new_size <= max) && (new_size >= 10)) {
    if ((width / 2) % new_size == 0 && (height / 2) % new_size == 0) {
      return new_size;
    }
    new_size += inc;
  }
  return current_size;
}

Vector2 convert_screen_to_graph_coords(Vector2 screen_coords, int width,
                                       int height, int grid_size) {
  Vector2 graph_coords;
  graph_coords.x = convert_x(screen_coords.x, width, grid_size);
  graph_coords.y = convert_y(screen_coords.y, height, grid_size);
  return graph_coords;
}

Vector2 convert_graph_to_screen_coords(Vector2 graph_coords, int width,
                                       int height, int grid_size) {
  Vector2 screen_coords;
  screen_coords.x = graph_coords.x * grid_size + width / 2;
  screen_coords.y = -graph_coords.y * grid_size + height / 2;
  return screen_coords;
}

void insert_new_data(Data *pdata, Vector2 p, int width, int height,
                     int grid_size) {
  DataPoint *new_node;
  new_node = malloc(sizeof(DataPoint));
  new_node->next = NULL;
  new_node->prev = NULL;
  new_node->smoothed = -1;
  new_node->n = pdata->n;
  new_node->screen_coords = p;
  new_node->graph_coords =
      convert_screen_to_graph_coords(p, width, height, grid_size);
  if (pdata->n == 0) {
    pdata->head = new_node;
    pdata->last = new_node;
  } else {
    new_node->prev = pdata->last;
    pdata->last->next = new_node;
    pdata->last = new_node;
  }
  pdata->n += 1;
}

void reset(Data *ptr) {
  DataPoint *current = ptr->head;
  ptr->n = 0;
  ptr->head = NULL;
  while (current != NULL) {
    DataPoint *next = current->next;
    free(current);
    current = next;
  }
}

void draw_points(DataPoint *l, int radius, Color color) {
  for (DataPoint *p = l; p != NULL; p = p->next) {
    DrawCircle(p->screen_coords.x, p->screen_coords.y, radius, color);
  }
}

void draw_grid(int grid_size) {
  for (int x = 0; x < WIDTH; x += grid_size) {
    DrawLine(x, 0, x, HEIGHT, DARKGRAY);
  }
  for (int y = 0; y < HEIGHT; y += grid_size) {
    DrawLine(0, y, WIDTH, y, DARKGRAY);
  }

  DrawLineEx((Vector2){WIDTH / 2, 0}, (Vector2){WIDTH / 2, HEIGHT}, 5,
             DARKGRAY);
  DrawLineEx((Vector2){0, HEIGHT / 2}, (Vector2){WIDTH, HEIGHT / 2}, 5,
             DARKGRAY);
}
