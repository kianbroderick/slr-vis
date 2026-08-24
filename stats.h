#include <raylib.h>
#include <stddef.h>
#ifndef SCREEN_DIMENSIONS
#define WIDTH 900
#define HEIGHT 600
#define SCREEN_DIMENSIONS
#endif
#define SCREEN 0
#define GRAPH 1

typedef struct DataPoint {
  Vector2 screen;
  Vector2 graph;
  struct DataPoint *next;
} DataPoint;

typedef struct SLRModel {
  float intercept;
  float slope;
} SLRModel;

DataPoint *insert_new_data(DataPoint *list, Vector2 p);
Vector2 offset(Vector2 screen_coords, int width, int height);
float convert_x(float x);
float convert_y(float y);

float calculate_xmean(DataPoint *list, int coords);

float calculate_ymean(DataPoint *list, int coords);

SLRModel *compute_slr(DataPoint *data, SLRModel *slr, int coords);
float calculate_sse(SLRModel *model, DataPoint *data, int coords);
float calculate_sst(SLRModel *model, DataPoint *data, int coords);

float predict(SLRModel *model, float x);

char *model_equation(SLRModel *model, char *buffer, size_t size);

// void print_data(DataPoint *data);
