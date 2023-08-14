#include "s21_matrix_supplementary.h"

#include <stdbool.h>
#include <stddef.h>

#include "s21_matrix.h"

bool s21_is_square(matrix_t *A) {
  if (A == NULL)
    return false;
  if (A->columns == A->rows)
    return true;
  return false;
}

int s21_extract_submatrix(matrix_t *A, int r, int c, matrix_t *result) {
  if (A == NULL || result == NULL || A->matrix == NULL)
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

// Compute the minors matrix, a matrix of determinants of submatrices.
int s21_calc_minors(matrix_t *A, matrix_t *result) {
  if (A == NULL || result == NULL || A->matrix == NULL)
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
