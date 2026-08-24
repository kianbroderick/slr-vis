#include "stats.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

Vector2 convert_screen_to_graph(Vector2 screen_coords, int width, int height,
                                int grid_size) {
  Vector2 graph_coords;
  graph_coords.x = (screen_coords.x - width / 2) / grid_size;
  graph_coords.y = ((height / 2) - screen_coords.y) / grid_size;
  return graph_coords;
}

float convert_x(float x, int width, int grid_size) {
  return (x - (width / 2)) / grid_size;
}
float convert_y(float y, int height, int grid_size) {
  return ((height / 2) - y) / grid_size;
}

DataPoint *insert_new_data(DataPoint *list, Vector2 p, int width, int height,
                           int grid_size) {
  DataPoint *new_node;
  new_node = malloc(sizeof(DataPoint));
  new_node->screen = p;
  new_node->graph = convert_screen_to_graph(p, width, height, grid_size);
  new_node->next = list;
  return new_node;
}

void reset(DataPoint **head) {
  DataPoint *current = *head;
  while (current != NULL) {
    DataPoint *next = current->next;
    free(current);
    current = next;
  }
  *head = NULL;
}

float calculate_xmean(DataPoint *list, int coords) {
  if (list == NULL) {
    puts("empty list in xmean");
  }
  int count = 0;
  float sum = 0;
  for (DataPoint *p = list; p != NULL; p = p->next) {
    count += 1;
    if (coords == SCREEN) {
      sum += p->screen.x;
    } else {
      sum += p->graph.x;
    }
  }
  return (sum / count);
}
float calculate_ymean(DataPoint *list, int coords) {
  if (list == NULL) {
    puts("empty list in ymean");
  }
  int count = 0;
  float sum = 0;
  for (DataPoint *p = list; p != NULL; p = p->next) {
    count += 1;
    if (coords == SCREEN) {
      sum += p->screen.y;
    } else {
      sum += p->graph.y;
    }
  }
  return (sum / count);
}

float calculate_slope(DataPoint *list, int coords) {
  float num = 0, denom = 0;
  float x_bar = calculate_xmean(list, coords);
  float y_bar = calculate_ymean(list, coords);
  for (DataPoint *p = list; p != NULL; p = p->next) {
    if (coords == SCREEN) {
      num += (p->screen.x - x_bar) * (p->screen.y - y_bar);
      denom += (p->screen.x - x_bar) * (p->screen.x - x_bar);
    } else {
      num += (p->graph.x - x_bar) * (p->graph.y - y_bar);
      denom += (p->graph.x - x_bar) * (p->graph.x - x_bar);
    }
  }
  return (num / denom);
}

float calculate_intercept(float x_bar, float y_bar, float slope) {
  return (y_bar - x_bar * slope);
}

SLRModel *compute_slr(DataPoint *data, SLRModel *slr, int coords) {
  float x_bar = calculate_xmean(data, coords);
  float y_bar = calculate_ymean(data, coords);
  slr->slope = calculate_slope(data, coords);
  slr->intercept = calculate_intercept(x_bar, y_bar, slr->slope);
  return slr;
}

float predict(SLRModel *model, float x) {
  float y;
  y = model->intercept + model->slope * x;
  return y;
}

char *model_equation(SLRModel *model, char *buffer, size_t size) {
  snprintf(buffer, size, "Y = %.f + %.2fX\n", model->intercept, model->slope);
  return buffer;
}

float calculate_sse(SLRModel *model, DataPoint *data, int coords) {
  float sse = 0;
  float residual;
  for (DataPoint *p = data; p != NULL; p = p->next) {
    if (coords == SCREEN) {
      residual = p->screen.y - predict(model, p->screen.x);
    } else {
      residual = p->graph.y - predict(model, p->graph.x);
    }
    sse += residual * residual;
  }
  return sse;
}

float calculate_sst(SLRModel *model, DataPoint *data, int coords) {
  float sst = 0;
  float t;
  float mean = calculate_ymean(data, coords);
  for (DataPoint *p = data; p != NULL; p = p->next) {
    if (coords == SCREEN) {
      t = p->screen.y - mean;
    } else {
      t = p->graph.y - mean;
    }
    sst += t * t;
  }
  return sst;
}
