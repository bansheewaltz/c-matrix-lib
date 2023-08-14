#include <math.h>
#include <stddef.h>

#include "s21_matrix.h"
#include "s21_matrix_supplementary.h"

#define SUCCESS 1
#define FAILURE 0
#define EPS 1e-7

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (!A || !A->matrix || !B || !B->matrix)
    return FAILURE;
  if (!s21_are_the_same_size(A, B))
    return FAILURE;
  for (int i = 0; i < A->columns; i++) {
    for (int j = 0; j < A->rows; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPS) {
        return FAILURE;
      }
    }
  }
  return SUCCESS;
}
