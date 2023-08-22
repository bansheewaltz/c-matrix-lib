#include "s21_matrix_supplementary.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#ifdef TEST_MALLOC
#include <stdlib.h>
#endif

#include "s21_matrix.h"

int s21_matrix_size(matrix_t *A) { return A->columns * A->rows; }
int s21_matrix_size_s(matrix_t *A, int *result) {
  if (A == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  *result = s21_matrix_size(A);
  return RC_OK;
}

bool s21_is_square(matrix_t *A) { return A->columns == A->rows; }
int s21_is_square_s(matrix_t *A, bool *result) {
  if (A == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  *result = s21_is_square(A);
  return RC_OK;
}

bool s21_is_diagonal(matrix_t *A) {
  if (!s21_is_square(A)) return false;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (i == j) continue;
      if (A->matrix[i][j] != 0) {
        return false;
      }
    }
  }
  return true;
}
int s21_is_diagonal_s(matrix_t *A, bool *result) {
  if (A == NULL || A->matrix == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  if (!s21_is_square(A)) {
    return RC_INCORRECT_MATRIX;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (i == j) continue;
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
  if (!A || !A->matrix || !result || !res_det) {
    return RC_NULL_POINTER_INPUT;
  }
  if (!s21_is_square(A)) {
    return RC_CALCULATIONS_CANNOT_BE_PERFORMED;
  }

  if (s21_is_diagonal(A)) {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i][i] == 0) {
        *result = true;
        return RC_OK;
      }
    }
  }

  // regular square matrix case
  (void)s21_determinant(A, res_det);  // all checks are made
  bool is_det_eq_zero = fabs(*res_det) < MTX_ACCURACY;
  *result = is_det_eq_zero;

  return RC_OK;
}

int s21_is_invertible(matrix_t *A, bool *result, double *res_det) {
  int rc = s21_is_singular(A, result, res_det);
  if (rc != RC_OK) {
    return rc;
  }
  *result = !(*result);
  return RC_OK;
}

bool s21_are_same_size(matrix_t *A, matrix_t *B) {
  return A->rows == B->rows && A->columns == B->columns;
}
int s21_are_same_size_s(matrix_t *A, matrix_t *B, bool *result) {
  if (A == NULL || B == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  *result = A->rows == B->rows && A->columns == B->columns;
  return RC_OK;
}

int s21_extract_submatrix(matrix_t *A, int r, int c, matrix_t *result) {
  if (A == NULL || A->matrix == NULL || result == NULL) {
    return RC_NULL_POINTER_INPUT;
  }
  if (r < 0 || r >= A->rows || c < 0 || c >= A->columns) {
    return RC_INCORRECT_MATRIX_PARAMETERS;
  }
  const int rows_res = A->rows - 1;
  const int cols_res = A->columns - 1;
  int rc = s21_create_matrix(rows_res, cols_res, result);
#ifdef TEST_MALLOC           // only malloc can fail here
  if (r == 13 && c == 17) {  // trigger values
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }

  for (int i = 0; i < rows_res; i++) {
    for (int j = 0; j < cols_res; j++) {
      int i_A = i;
      if (i >= r) i_A += 1;
      int j_A = j;
      if (j >= c) j_A += 1;
      result->matrix[i][j] = A->matrix[i_A][j_A];
    }
  }

  return RC_OK;
}

int s21_calc_minors(matrix_t *A, matrix_t *result) {
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

  const int rows_res = A->rows;
  const int cols_res = A->columns;
  rc = s21_create_matrix(rows_res, cols_res, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->matrix[0][0] == 11) {  // trigger value
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      matrix_t submatrix;
      rc = s21_extract_submatrix(A, i, j, &submatrix);
#ifdef TEST_MALLOC
      if (rc == RC_OK && A->matrix[0][1] == 13) {  // trigger value
        s21_remove_matrix(&submatrix);
        rc = RC_MEMORY_ALLOCATION_FAILED;
      }
#endif
      if (rc == RC_MEMORY_ALLOCATION_FAILED) {
        s21_remove_matrix(result);
      }
      if (rc != RC_OK) {
        return rc;
      }

      double determinant;
      (void)s21_determinant(&submatrix, &determinant);  // all checks are made
      result->matrix[i][j] = determinant;
      s21_remove_matrix(&submatrix);
    }
  }

  return RC_OK;
}

int s21_adjugate(matrix_t *A, matrix_t *result) {
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

  matrix_t cofactor;
  rc = s21_calc_complements(A, &cofactor);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->matrix[0][0] == 41) {  // trigger value
    s21_remove_matrix(&cofactor);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  if (rc != RC_OK) {
    return rc;
  }
  rc = s21_transpose(&cofactor, result);
#ifdef TEST_MALLOC
  if (rc == RC_OK && A->matrix[0][1] == 23) {  // trigger value
    s21_remove_matrix(result);
    rc = RC_MEMORY_ALLOCATION_FAILED;
  }
#endif
  s21_remove_matrix(&cofactor);
  if (rc != RC_OK) {
    return rc;
  }
  return RC_OK;
}
