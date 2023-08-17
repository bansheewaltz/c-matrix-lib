#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

// In case of a failure in malloc, cleans up already allocated matrix arrays
void s21_cleanup_matrix(matrix_t *A);

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (result == NULL)
    return RC_NULL_POINTER_INPUT;
  if (rows < 1 || columns < 1)
    return RC_INCORRECT_MATRIX_PARAMETERS;

  result->matrix = calloc((unsigned)rows, sizeof(double *));
#ifdef TESTING
  if (rows == 137 && columns == 139) {
    free(result->matrix);
    result->matrix = NULL;
  }
#endif
  if (result->matrix == NULL)
    return RC_MEMORY_ALLOCATION_FAILED;

  result->rows = rows;
  result->columns = columns;

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = calloc((unsigned)columns, sizeof(double));
#ifdef TESTING
    if (rows == 139 && columns == 137 && i > 0) {
      free(result->matrix[i]);
      result->matrix[i] = NULL;
    }
#endif
    if (result->matrix[i] == NULL) {
      s21_cleanup_matrix(result);
      return RC_MEMORY_ALLOCATION_FAILED;
    }
  }

  return RC_OK;
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

void s21_cleanup_matrix(matrix_t *A) {
  for (int i = 0; A->matrix[i] != NULL; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
}
