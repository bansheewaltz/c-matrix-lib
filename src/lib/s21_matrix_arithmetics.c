#include "s21_matrix.h"
#include "s21_matrix_supplementary.h"

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !A->matrix || !B || !B->matrix || !result) {
    return RC_NULL_POINTER_INPUT;
  }
  if (!s21_are_same_size(A, B)) {
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;
  }

  const int rows_res = A->rows;
  const int cols_res = A->columns;
  int rc = s21_create_matrix(rows_res, cols_res, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->rows == 11 && A->columns == 13) {  // trigger value
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }

  for (int i = 0; i < rows_res; i++) {
    for (int j = 0; j < cols_res; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }

  return RC_OK;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !A->matrix || !B || !B->matrix || !result) {
    return RC_NULL_POINTER_INPUT;
  }
  if (!s21_are_same_size(A, B)) {
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;
  }

  const int rows_res = A->rows;
  const int cols_res = A->columns;
  int rc = s21_create_matrix(rows_res, cols_res, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->rows == 11 && A->columns == 13) {  // trigger value
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }

  for (int i = 0; i < rows_res; i++) {
    for (int j = 0; j < cols_res; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }

  return RC_OK;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (!A || !A->matrix || !result) {
    return RC_NULL_POINTER_INPUT;
  }

  const int rows_res = A->rows;
  const int cols_res = A->columns;
  int rc = s21_create_matrix(rows_res, cols_res, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && number == 1.0 / 16) {  // trigger value
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }

  for (int i = 0; i < rows_res; i++) {
    for (int j = 0; j < cols_res; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }

  return RC_OK;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !A->matrix || !B || !B->matrix || !result) {
    return RC_NULL_POINTER_INPUT;
  }
  if (A->rows != B->columns) {
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;
  }

  const int rows_res = A->rows;
  const int cols_res = B->columns;
  int rc = s21_create_matrix(rows_res, cols_res, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->matrix[0][0] == 5 && B->matrix[0][0] == 7) {
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }

  for (int i = 0; i < rows_res; i++) {
    for (int j = 0; j < cols_res; j++) {
      double sum = 0;
      for (int k = 0; k < B->rows; k++) {
        sum += A->matrix[i][k] * B->matrix[k][j];
      }
      result->matrix[i][j] = sum;
    }
  }

  return RC_OK;
}
