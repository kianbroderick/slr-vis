#include "slr.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

float calculate_xmean(DataPoint *list, int coords) {
  if (list == NULL) {
    puts("empty list in xmean");
  }
  int count = 0;
  float sum = 0;
  for (DataPoint *p = list; p != NULL; p = p->next) {
    count += 1;
    if (coords == SCREEN) {
      sum += p->screen_coords.x;
    } else {
      sum += p->graph_coords.x;
    }
  }
  // printf("Calculated xmean = %.2f\n", sum / count);
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
      sum += p->screen_coords.y;
    } else {
      sum += p->graph_coords.y;
    }
  }
  // printf("Calculated ymean = %.2f\n", sum / count);
  return (sum / count);
}

float calculate_slope(DataPoint *list, int coords) {
  float num = 0, denom = 0;
  float x_bar = calculate_xmean(list, coords);
  float y_bar = calculate_ymean(list, coords);
  for (DataPoint *p = list; p != NULL; p = p->next) {
    if (coords == SCREEN) {
      num += (p->screen_coords.x - x_bar) * (p->screen_coords.y - y_bar);
      denom += (p->screen_coords.x - x_bar) * (p->screen_coords.x - x_bar);
    } else {
      num += (p->graph_coords.x - x_bar) * (p->graph_coords.y - y_bar);
      denom += (p->graph_coords.x - x_bar) * (p->graph_coords.x - x_bar);
    }
  }
  // printf("Calculated slope as %.2f\n", num / denom);
  return (num / denom);
}

float calculate_intercept(float x_bar, float y_bar, float slope) {
  // printf("Calculated slope = %.2f\n", y_bar - x_bar * slope);
  return (y_bar - x_bar * slope);
}

SLRModel *compute_slr(DataPoint *data, int coords) {
  SLRModel *slr = malloc(sizeof *slr);
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
      residual = p->screen_coords.y - predict(model, p->screen_coords.x);
    } else {
      residual = p->graph_coords.y - predict(model, p->graph_coords.x);
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
      t = p->screen_coords.y - mean;
    } else {
      t = p->graph_coords.y - mean;
    }
    sst += t * t;
  }
  return sst;
}

void draw_line(SLRModel *model, int width) {
  float end_y = model->slope * width + model->intercept;
  DrawLineEx((Vector2){0, model->intercept}, (Vector2){width, end_y}, 3, LIME);
}

void draw_model_text(Data *pdata) {
  if (pdata->head != NULL && pdata->n > 1) {
    SLRModel *screen_model = compute_slr(pdata->head, SCREEN);
    SLRModel *graph_model = compute_slr(pdata->head, GRAPH);
    float sst = calculate_sst(graph_model, pdata->head, GRAPH);
    float sse = calculate_sse(graph_model, pdata->head, GRAPH);
    draw_line(screen_model, WIDTH);
    DrawText(TextFormat("Y = %.2f + %.2fX\n", graph_model->intercept,
                        graph_model->slope),
             10, 10, 30, BLACK);
    DrawText(TextFormat("R^2 = %.2f\n", 1 - (sse / sst)), 10, 60, 30, BLACK);
  }
}
