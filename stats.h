#include <raylib.h>
#include <stddef.h>
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

DataPoint *insert_new_data(DataPoint *list, Vector2 p, int width, int height,
                           int grid_size);
Vector2 offset(Vector2 screen_coords, int width, int height);
float convert_x(float x, int width, int grid_size);
float convert_y(float y, int height, int grid_size);

float calculate_xmean(DataPoint *list, int coords);

float calculate_ymean(DataPoint *list, int coords);

SLRModel *compute_slr(DataPoint *data, SLRModel *slr, int coords);
float calculate_sse(SLRModel *model, DataPoint *data, int coords);
float calculate_sst(SLRModel *model, DataPoint *data, int coords);

float predict(SLRModel *model, float x);

char *model_equation(SLRModel *model, char *buffer, size_t size);

// void print_data(DataPoint *data);
