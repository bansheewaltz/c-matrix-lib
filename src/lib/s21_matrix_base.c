#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (result == NULL)
    return RC_NULL_POINTER_INPUT;
  if (rows < 1 || columns < 1)
    return RC_INCORRECT_MATRIX_PARAMETERS;

  result->matrix = calloc(columns, sizeof(double *));
  if (result->matrix == NULL)
    return RC_MEMORY_ALLOCATION_FAILED;

  int rc = RC_OK;

  result->rows = rows;
  result->columns = columns;

  for (int i = 0; i < columns; i++) {
    result->matrix[i] = (double *)calloc(rows, sizeof(double));
    if (result->matrix[i] == NULL) {
      rc = RC_MEMORY_ALLOCATION_FAILED;
      break;
    }
  }

  return rc;
}

void s21_remove_matrix(matrix_t *A) {
  if (A == NULL)
    return;
  if (A->matrix) {
    for (int i = 0; i < A->columns; i++) {
      free(A->matrix[i]);
    }
  }
  free(A->matrix);
}
