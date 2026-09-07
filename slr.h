#include "plotting.h"
#include <raylib.h>
#include <stddef.h>
#define SCREEN 0
#define GRAPH 1

void draw_line(SLRModel *model, int width);
// Vector2 offset(Vector2 screen_coords, int width, int height);
float convert_x(float x, int width, int grid_size);
float convert_y(float y, int height, int grid_size);

float calculate_xmean(DataPoint *list, int coords);
float calculate_ymean(DataPoint *list, int coords);

SLRModel *compute_slr(DataPoint *data, int coords);
float calculate_sse(SLRModel *model, DataPoint *data, int coords);
float calculate_sst(SLRModel *model, DataPoint *data, int coords);

float predict(SLRModel *model, float x);

char *model_equation(SLRModel *model, char *buffer, size_t size);
void draw_model_text(Data *pdata);
