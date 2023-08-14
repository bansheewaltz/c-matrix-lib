#ifndef S21_MATRIX_SUPPLEMENTARY_H_
#define S21_MATRIX_SUPPLEMENTARY_H_

#include <stdbool.h>

#include "s21_matrix.h"

int s21_calc_minors(matrix_t *A, matrix_t *result);
int s21_extract_submatrix(matrix_t *A, int r, int c, matrix_t *result);
bool s21_is_square(matrix_t *A);

#endif  // S21_MATRIX_SUPPLEMENTARY_H_
