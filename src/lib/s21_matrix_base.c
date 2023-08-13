#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows < 1 || columns < 1)
    return RC_OPERATION_ERROR;

  result->matrix = calloc(columns, sizeof(double *));
  if (result->matrix == NULL)
    return RC_INTERNAL_ERROR;

  int rc = RC_OK;

  result->rows = rows;
  result->columns = columns;

  for (int i = 0; i < columns; i++) {
    result->matrix[i] = (double *)calloc(rows, sizeof(double));
    if (result->matrix[i] == NULL) {
      rc = RC_INTERNAL_ERROR;
      break;
    }
  }

  return rc;
}

void s21_remove_matrix(matrix_t *A) {
  if (A == NULL || A->matrix == NULL)
    return;
  for (int i = 0; i < A->columns; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
}
