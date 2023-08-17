#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (result == NULL)
    return RC_NULL_POINTER_INPUT;
  if (rows < 1 || columns < 1)
    return RC_INCORRECT_MATRIX_PARAMETERS;

  result->matrix = calloc((unsigned)rows, sizeof(double *));
#ifdef TESTING
  if (rows == 137 && columns == 139) {
    result->matrix = NULL;
  }
#endif
  if (result->matrix == NULL)
    return RC_MEMORY_ALLOCATION_FAILED;

  int rc = RC_OK;

  result->rows = rows;
  result->columns = columns;

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = calloc((unsigned)columns, sizeof(double));
#ifdef TESTING
    if (rows == 139 && columns == 137) {
      result->matrix[i] = NULL;
    }
#endif
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
  if (A->matrix == NULL)
    return;
  for (int i = 0; i < A->rows; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
}
