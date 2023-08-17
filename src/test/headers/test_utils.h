#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>

double get_rand_double(double min, double max) {
  // 0 to 1
  double val1 = (double)rand() / RAND_MAX;
  // 0 to (max - min)
  double val2 = (max - min) * val1;
  // min to max
  double val3 = val2 + min;
  return val3;
}

// 0 to n inclusive
int get_rand_pos_int(int max) {
  // attempt at uniformity
  int top = max + 1;
  int end = RAND_MAX / top;  // truncate skew
  end *= top;
  int r = 0;
  while ((r = rand()) > end)
    ;
  return r % top;
}

#endif  // UTILS_H_
