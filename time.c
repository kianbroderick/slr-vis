#include "time.h"
#include "raylib.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

double avg_last_k(DataPoint *p, int k) {
  double sum = 0;
  int n = p->n;
  for (int i = 0; p != NULL && i < k; p = p->prev, i++) {
    sum += p->screen_coords.y;
  }
  return sum / k;
}

void smooth_moving_average(DataPoint *head, int k) {
  DataPoint *p = head;
  for (int i = 0; p != NULL && i < k; i++) {
    p = p->next;
  }
  for (; p != NULL; p = p->next) {
    p->smoothed = avg_last_k(p, k);
  }
}

void smoothing_exponential(DataPoint *head, double w) {
  DataPoint *p = head;
  p->smoothed = (1 - w) * p->graph_coords.y;
  p = p->next;
  for (; p != NULL; p = p->next) {
    p->smoothed = (1 - w) * p->screen_coords.y + w * p->prev->smoothed;
  }
}

void draw_avg_smooth_predictions(DataPoint *head, int k, int radius,
                                 Color color) {
  DataPoint *p = head;
  int i = 1;
  for (p = head; p != NULL; p = p->next, i++) {
    if (i >= k) {
      DrawCircle(p->screen_coords.x, p->smoothed, radius, color);
    }
  }
}

void draw_exp_smooth_predictions(DataPoint *head, int radius, Color color) {
  DataPoint *p = head;
  for (p = head; p != NULL; p = p->next) {
    DrawCircle(p->screen_coords.x, p->smoothed, radius, color);
  }
}
