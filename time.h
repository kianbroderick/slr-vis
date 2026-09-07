#include "plotting.h"

double avg_last_k(DataPoint *p, int k);
void smooth_moving_average(DataPoint *head, int k);
void smoothing_exponential(DataPoint *head, double w);
void draw_avg_smooth_predictions(DataPoint *head, int k, int radius,
                                 Color color);
void draw_exp_smooth_predictions(DataPoint *head, int radius, Color color);
