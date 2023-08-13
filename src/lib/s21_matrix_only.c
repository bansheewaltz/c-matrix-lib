#include "math.h"
#include "s21_matrix.h"
#include "s21_matrix_extra.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
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

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (A->columns != A->rows)
    return RC_OPERATION_ERROR;

  int rc = s21_create_matrix(A->columns, A->rows, result);
  if (rc != RC_OK)
    return rc;

  for (int i = 0; i < result->columns; i++) {
    for (int j = 0; j < result->rows; j++) {
      matrix_t mtx_deg;
      double d = 0.0;
      matrix_degrade(A, &mtx_deg, i, j);
      s21_determinant(&mtx_deg, &d);
      result->matrix[i][j] += pow(-1, i + j) * d;
      s21_remove_matrix(&mtx_deg);
    }
  }
  return err;
}

int s21_determinant(matrix_t *A, double *result) {
  if (A->columns != A->rows)
    return RC_OPERATION_ERROR;

  if (A->columns == 1) {
    *result = A->matrix[0][0];
    return RC_OK;
  }
  if (A->columns == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    return RC_OK;
  }

  for (int i = 0; i < A->columns; i++) {
    matrix_t mtx_deg;
    double d = 0.0;
    matrix_degrade(A, &mtx_deg, i, 0);
    s21_determinant(&mtx_deg, &d);
    *result += A->matrix[i][0] * pow(-1, i) * d;
    s21_remove_matrix(&mtx_deg);
  }

  return 0;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
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
