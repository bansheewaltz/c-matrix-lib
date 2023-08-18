#include "s21_matrix.h"
#include "s21_matrix_supplementary.h"

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !A->matrix || !B || !B->matrix || !result)
    return RC_NULL_POINTER_INPUT;
  if (!s21_are_same_size(A, B))
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;

  int rc = s21_create_matrix(A->rows, A->columns, result);
#ifdef TESTING
  if (A->rows == 11 && A->columns == 13) {
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK)
    return rc;

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }

  return RC_OK;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !A->matrix || !B || !B->matrix || !result)
    return RC_NULL_POINTER_INPUT;
  if (s21_are_same_size(A, B))
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;

  int rc = s21_create_matrix(A->rows, A->columns, result);
  if (rc != RC_OK)
    return rc;

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }

  return RC_OK;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (!A || !A->matrix || !result)
    return RC_NULL_POINTER_INPUT;

  int rc = s21_create_matrix(A->rows, A->columns, result);
  if (rc != RC_OK)
    return rc;

  for (int i = 0; i < result->columns; i++) {
    for (int j = 0; j < result->rows; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }

  return RC_OK;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !A->matrix || !B || !B->matrix || !result)
    return RC_NULL_POINTER_INPUT;
  if (A->rows != B->columns)
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;

  int rc = s21_create_matrix(B->rows, A->columns, result);
  if (rc != RC_OK)
    return rc;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      double sum = 0;
      for (int k = 0; k < A->columns; k++) {
        sum += A->matrix[i][k] * B->matrix[k][i];
      }
      result->matrix[j][i] = sum;
    }
  }

  return RC_OK;
}
