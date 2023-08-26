
#include <stddef.h>
#include <stdio.h>

#include "s21_matrix.h"
#include "s21_matrix_supplementary.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  const int rows_res = A->columns;
  const int cols_res = A->rows;
  int rc = s21_create_matrix(rows_res, cols_res, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->matrix[0][0] == 13) {  // trigger value
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }
  for (int i = 0; i < rows_res; i++) {
    for (int j = 0; j < cols_res; j++) {
      result->matrix[i][j] = A->matrix[j][i];
    }
  }
  return RC_OK;
}

int s21_determinant(matrix_t *A, double *result) {
  if (A == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  if (!s21_is_square(A)) {
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;
  }
  const int side_len = A->rows;

  if (side_len == 0) {
    *result = 1;
    return RC_OK;
  }
  if (A->matrix == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  if (side_len == 1) {
    *result = A->matrix[0][0];
    return RC_OK;
  }
  if (side_len == 2) {
    double **m = A->matrix;
    *result = m[0][0] * m[1][1] - m[0][1] * m[1][0];
    return RC_OK;
  }
  if (s21_is_diagonal(A)) {
    double diagonal_product = 1.0;
    for (int i = 0; i < side_len; i++) {
      diagonal_product *= A->matrix[i][i];
    }
    *result = diagonal_product;
    return RC_OK;
  }

  // general case of a square matrix
  double det = 0.0;
  int sign = 1;
  for (int i = 0; i < side_len; i++) {
    matrix_t subm;
    int rc = RC_OK;
    rc = s21_extract_submatrix(A, 0, i, &subm);
    if (rc != RC_OK) {
      return rc;
    }
    double subm_det;
    rc = s21_determinant(&subm, &subm_det);
    if (rc != RC_OK) {
      return rc;
    }
    s21_remove_matrix(&subm);
    det += A->matrix[0][i] * subm_det * sign;
    sign = -sign;
  }
  *result = det;
  return RC_OK;
}

// Compute the cofactor matrix, the Hadamard multiplication (element-wise
// product) of the minors matrix with an alternating grid of +1 and −1.
int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  if (!s21_is_square(A)) {
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;
  }
  const int side_len = A->rows;

  int rc = RC_OK;

  if (side_len == 1) {
    rc = s21_create_matrix(1, 1, result);
    if (rc != RC_OK) {
      return rc;
    }
    result->matrix[0][0] = 1;
    return RC_OK;
  }

  rc = s21_calc_minors(A, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->matrix[0][0] == 117) {  // trigger value
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }

  // Hadamard multiplication
  int matrix_size = side_len * side_len;
  for (int i = 1; i < matrix_size; i += 2) {
    result->matrix[i / A->rows][i % A->columns] *= -1;
  }

  return RC_OK;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  if (!s21_is_square(A)) {
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;
  }

  int rc = RC_OK;

  bool is_invertible = false;
  double determinant = 0.0;
  (void)s21_is_invertible(A, &is_invertible, &determinant);  // all checks are
  if (is_invertible == false) {                              // made
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;
  }

  if (s21_matrix_size(A) == 1) {
    rc = s21_create_matrix(1, 1, result);
#ifdef TEST_MALLOC
    if (A->matrix[0][0] == 14) {  // trigger value
      s21_remove_matrix(result);
      rc = RC_MEMORY_ALLOCATION_FAILED;
    }
#endif
    if (rc != RC_OK) {
      return rc;
    }
    result->matrix[0][0] = 1.0 / A->matrix[0][0];
    return RC_OK;
  }

  // general case of an invertable matrix
  matrix_t adjugate;
  rc = s21_adjugate(A, &adjugate);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->matrix[0][0] == 14) {  // trigger value
    s21_remove_matrix(&adjugate);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }
  rc = s21_mult_number(&adjugate, 1.0 / determinant, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->matrix[0][0] == 15) {  // trigger value
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  s21_remove_matrix(&adjugate);
  return rc;
}
