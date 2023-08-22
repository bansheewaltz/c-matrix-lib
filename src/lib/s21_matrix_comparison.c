#include <math.h>
#include <stddef.h>

#include "s21_comparison_defines.h"
#include "s21_matrix.h"
#include "s21_matrix_supplementary.h"

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (!A || !A->matrix || !B || !B->matrix) {
    return FAILURE;
  }
  if (!s21_are_same_size(A, B)) {
    return FAILURE;
  }
  const int rows = A->rows;
  const int cols = A->columns;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double error = fabs(A->matrix[i][j] - B->matrix[i][j]);
      if (error > EPS_EQ) {
        return FAILURE;
      }
    }
  }
  return SUCCESS;
}
