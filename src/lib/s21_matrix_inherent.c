
#include <stddef.h>

#include "s21_matrix.h"
#include "s21_matrix_supplementary.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  int rc = s21_create_matrix(A->columns, A->rows, result);
  if (rc != RC_OK)
    return rc;
  for (int i = 0; i < result->columns; i++) {
    for (int j = 0; j < result->rows; j++) {
      result->matrix[i][j] = A->matrix[j][i];
    }
  }
  return RC_OK;
}

// through permutations
int s21_determinant(matrix_t *A, double *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  if (!s21_is_square(A))
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;

  int matrix_size = A->columns;
  if (matrix_size == 1) {
    *result = A->matrix[0][0];
    return RC_OK;
  }

  double pos_diagonal_sum = 0.0;
  double neg_diagonal_sum = 0.0;
  for (int i = 0; i < A->columns; i++) {
    double pos_diagonal = 1.0;
    double neg_diagonal = 1.0;
    for (int j = 0; j < matrix_size; j++) {
      pos_diagonal *= A->matrix[(i + j) % matrix_size][(i + j) % matrix_size];
      neg_diagonal *= A->matrix[(i - j) % matrix_size][(i - j) % matrix_size];
    }
    pos_diagonal_sum += pos_diagonal;
    neg_diagonal_sum += neg_diagonal;
  }
  *result = pos_diagonal_sum - neg_diagonal_sum;

  return RC_OK;
}

// Compute the cofactors matrix, the Hadamard multiplication of the minors
// matrix with an alternating grid of +1 and −1.
int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;
  if (!s21_is_square(A))
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;

  int rc = RC_OK;

  rc = s21_create_matrix(A->columns, A->rows, result);
  if (rc != RC_OK)
    return rc;

  rc = s21_calc_minors(A, result);
  if (rc != RC_OK)
    return rc;

  int matrix_size = A->rows * A->rows;
  for (int i = 1; i < matrix_size; i += 2) {
  }

  return rc;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL)
    return RC_NULL_POINTER_INPUT;

  int err = 0;
  err = (A->columns != A->rows);
  double det = 0.0;
  s21_determinant(A, &det);
  if (det == 0 && !err) {
    err = 2;
  }
  if (!err) {
    if (A->columns == 1) {
      s21_create_matrix(1, 1, result);
      result->matrix[0][0] = 1.0 / A->matrix[0][0];
    } else {
      matrix_t tmp_minor, tmp_trans;
      s21_calc_complements(A, &tmp_minor);
      s21_transpose(&tmp_minor, &tmp_trans);
      err = s21_mult_number(&tmp_trans, 1.0 / det, result);
      s21_remove_matrix(&tmp_minor);
      s21_remove_matrix(&tmp_trans);
    }
  } else {
    s21_create_matrix(A->rows, A->columns, result);
  }

  return err;
}
