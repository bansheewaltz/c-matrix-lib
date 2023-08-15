#ifndef S21_MATRIX_SUPPLEMENTARY_H_
#define S21_MATRIX_SUPPLEMENTARY_H_

#include <stdbool.h>

#include "s21_matrix.h"

// Calculates the size of the matrix A as its row count multiplied by its
// column count.
int s21_matrix_size(matrix_t *A);
// Safe version of s21_matrix_size_s(...).
int s21_matrix_size_s(matrix_t *A, int *result);

// Calculates the minors matrix (a matrix of determinants of submatrices) of
// the matrix A.
int s21_calc_minors(matrix_t *A, matrix_t *result);

// Calculates the adjoint of the square matrix A (the transpose of its cofactor
// matrix).
int s21_adjugate(matrix_t *A, matrix_t *result);

// Result is the matrix excluding the r-th row and the c-th column.
// Checks the arguments of pointer type for NULL value.
int s21_extract_submatrix(matrix_t *A, int r, int c, matrix_t *result);

// Checks that the matrix A is square (row count is equal to column count).
bool s21_is_square(matrix_t *A);
// Safe version of s21_is_square(...).
int s21_is_square_s(matrix_t *A, bool *result);

// Checks that the matrix A is diagonal (a square matrix in which every element
// except the principal diagonal elements is zero).
bool s21_is_diagonal(matrix_t *A);
// Safe version of s21_is_diagonal(...).
int s21_is_diagonal_s(matrix_t *A, bool *result);

// Ckecks that the matrix A is singular (a square matrix is singular if and
// only if its determinant is 0)
int s21_is_singular(matrix_t *A, bool *result, double *res_det);

// Checks that the matrix A is invertible (a matrix whose inverse exists).
// Specifically in this implementation it calls !s21_is_singular(...)
int s21_is_invertible(matrix_t *A, bool *result, double *res_det);

// Checks that the two matrices A and B are the same size.
bool s21_are_same_size(matrix_t *A, matrix_t *B);
// Safe version of s21_are_same_size(...).
int s21_are_same_size_s(matrix_t *A, matrix_t *B, bool *result);

#endif  // S21_MATRIX_SUPPLEMENTARY_H_
