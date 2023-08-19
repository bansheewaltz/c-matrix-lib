#ifndef UTILS_H_
#define UTILS_H_

#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

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

void cleanup_matrix_2d_array(matrix_t *m) {
  for (int i = 0; i < m->rows; i++) {
    free(m->matrix[i]);
  }
  free(m->matrix);
  m->matrix = NULL;
}

void cleanup_matrix_rows_arrays(matrix_t *m) {
  for (int i = 0; i < m->rows; i++) {
    free(m->matrix[i]);
  }
}

void print_matrix(matrix_t *m) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->columns; j++) {
      printf("%f ", m->matrix[i][j]);
    }
    printf("\n");
  }
}

void fill_matrix_with_rand(matrix_t *m) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->columns; j++) {
      m->matrix[i][j] = get_rand_double(DBL_MIN, DBL_MAX);
    }
  }
}

#endif  // UTILS_H_
