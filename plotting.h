#pragma once
#include "raylib.h"
#define WIDTH 900
#define HEIGHT 600
#define GRID_SIZE 50

#define SLR 0
#define AVERAGE_SMOOTHING 1
#define EXPONENTIAL_SMOOTHING 2

float convert_x(float x, int width, int grid_size);
float convert_y(float y, int height, int grid_size);

Vector2 convert_screen_to_graph_coords(Vector2 screen_coords, int width,
                                       int height, int grid_size);

Vector2 convert_graph_to_screen_coords(Vector2 graph_coords, int width,
                                       int height, int grid_size);
int update_grid_size(int current_size, bool increasing, int width, int height);

typedef struct DataPoint {
  Vector2 screen_coords;
  Vector2 graph_coords;
  double smoothed;
  int n;
  struct DataPoint *prev;
  struct DataPoint *next;
} DataPoint;

typedef struct Data {
  int mode;
  int n;
  DataPoint *head;
  DataPoint *last;
} Data;

typedef struct SLRModel {
  float intercept;
  float slope;
} SLRModel;

void insert_new_data(Data *pdata, Vector2 p, int width, int height,
                     int grid_size);

void reset(Data *ptr);
void draw_points(DataPoint *l, int radius, Color color);
void draw_grid(int grid_size);
