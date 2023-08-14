#include "s21_matrix_supplementary.h"

#include <stdbool.h>
#include <stddef.h>

#include "s21_matrix.h"

bool s21_is_square(matrix_t *A) {
  if (A == NULL)
    return false;
  if (A->columns != A->rows)
    return false;
  return true;
}

bool s21_is_diagonal(matrix_t *A) {
  if (A == NULL || A->matrix == NULL)
    return false;
  if (!s21_is_square(A))
    return false;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->rows; j++) {
      if (i == j)
        continue;
      if (A->matrix[i][j] != 0)
        return false;
    }
  }
  return true;
}

bool s21_is_singular(matrix_t *A) {
  if (A == NULL || A->matrix == NULL)
    return false;

  if (!s21_is_square(A))
    return false;

  if (s21_is_diagonal(A)) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = i; j < A->columns; j++) {
        if (i != j)
          break;
        if (A->matrix[i][j] == 0)
          return true;
      }
    }
  }

  // regular square matrix case
  double det = 0.0;
  int rc = s21_determinant(A, &det);
  if (rc != RC_OK)
    return false;
  return det == 0.0;
}

bool s21_is_invertable(matrix_t *A) {
  return !s21_is_singular(A);
}

bool s21_are_the_same_size(matrix_t *A, matrix_t *B) {
  if (A == NULL || B == NULL)
    return false;
  if (A->rows != B->rows || A->columns != B->columns)
    return false;
  return true;
}

int s21_extract_submatrix(matrix_t *A, int r, int c, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  if (r < 0 || r >= A->rows || c < 0 || c >= A->columns)
    return RC_INCORRECT_MATRIX_PARAMETERS;

  int rc = s21_create_matrix(A->rows - 1, A->columns - 1, result);
  if (rc != RC_OK)
    return rc;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i >= r ? i + 1 : i][j >= r ? j + 1 : j] = A->matrix[i][j];
    }
  }

  return RC_OK;
}

int s21_calc_minors(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;

  int rc = RC_OK;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      matrix_t submatrix;
      rc = s21_extract_submatrix(A, i, j, &submatrix);
      if (rc != RC_OK)
        goto out;

      double determinant = 0.0;
      rc = s21_determinant(&submatrix, &determinant);
      if (rc != RC_OK)
        goto out;

      result->matrix[i][j] = determinant;
    }
  }

out:
  return rc;
}
