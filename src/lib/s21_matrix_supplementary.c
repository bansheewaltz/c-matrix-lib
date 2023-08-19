#include "s21_matrix_supplementary.h"

#include <stdbool.h>
#include <stddef.h>
#ifdef TESTING
#include <stdlib.h>
#endif

#include "s21_matrix.h"

int s21_matrix_size(matrix_t *A) {
  return A->columns * A->rows;
}
int s21_matrix_size_s(matrix_t *A, int *result) {
  if (A == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  *result = s21_matrix_size(A);
  return RC_OK;
}

bool s21_is_square(matrix_t *A) {
  return A->columns == A->rows;
}
int s21_is_square_s(matrix_t *A, bool *result) {
  if (A == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  *result = s21_is_square(A);
  return RC_OK;
}

bool s21_is_diagonal(matrix_t *A) {
  if (!s21_is_square(A))
    return false;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (i == j)
        continue;
      if (A->matrix[i][j] != 0)
        return false;
    }
  }
  return true;
}
int s21_is_diagonal_s(matrix_t *A, bool *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  if (!s21_is_square(A))
    return RC_INCORRECT_MATRIX;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (i == j)
        continue;
      if (A->matrix[i][j] != 0) {
        *result = false;
        return RC_OK;
      }
    }
  }

  *result = true;
  return RC_OK;
}

int s21_is_singular(matrix_t *A, bool *result, double *res_det) {
  if (A == NULL || A->matrix == NULL)
    return RC_NULL_POINTER_INPUT;
  if (!s21_is_square(A))
    return RC_INCORRECT_MATRIX;

  if (s21_is_diagonal(A)) {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i][i] == 0) {
        *result = true;
        return RC_OK;
      }
    }
  }

  // regular square matrix case
  int rc = s21_determinant(A, res_det);
  if (rc != RC_OK)
    return rc;
  *result = (*res_det == 0.0);
  return RC_OK;
}

int s21_is_invertible(matrix_t *A, bool *result, double *res_det) {
  return !s21_is_singular(A, result, res_det);
}

bool s21_are_same_size(matrix_t *A, matrix_t *B) {
  return A->rows == B->rows && A->columns == B->columns;
}
int s21_are_same_size_s(matrix_t *A, matrix_t *B, bool *result) {
  if (A == NULL || B == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  *result = A->rows == B->rows && A->columns == B->columns;
  return RC_OK;
}

int s21_extract_submatrix(matrix_t *A, int r, int c, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  if (r < 0 || r >= A->rows || c < 0 || c >= A->columns)
    return RC_INCORRECT_MATRIX_PARAMETERS;

  int rc = s21_create_matrix(A->rows - 1, A->columns - 1, result);
#ifdef TESTING
  if (r == 13 && c == 17) {
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK)
    return rc;

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      int i_A = i;
      if (i >= r)
        i_A += 1;
      int j_A = j;
      if (j >= c)
        j_A += 1;
      result->matrix[i][j] = A->matrix[i_A][j_A];
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
        return rc;

      double determinant = 0.0;
      rc = s21_determinant(&submatrix, &determinant);
      if (rc != RC_OK)
        return rc;

      result->matrix[i][j] = determinant;
    }
  }

  return rc;
}

int s21_adjugate(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  int rc = RC_OK;
  matrix_t cofactor;
  rc = s21_calc_complements(A, &cofactor);
  if (rc != RC_OK)
    return rc;
  rc = s21_transpose(&cofactor, result);
  if (rc != RC_OK)
    return rc;
  return RC_OK;
}
