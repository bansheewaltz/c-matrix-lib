#ifndef S21_MATRIX_SUPPLEMENTARY_H_
#define S21_MATRIX_SUPPLEMENTARY_H_

#include <stdbool.h>

#include "s21_matrix.h"

// Calculates the minors matrix, a matrix of determinants of submatrices.
// Checks the arguments of pointer type for NULL value.
int s21_calc_minors(matrix_t *A, matrix_t *result);

// Result is the matrix excluding the r-th row and the c-th column.
// Checks the arguments of pointer type for NULL value.
int s21_extract_submatrix(matrix_t *A, int r, int c, matrix_t *result);

// Checks that the matrix is square (rows count is equal to columns count).
// Also, returns false if the matrix pointer is NULL.
bool s21_is_square(matrix_t *A);

// Checks that the two matrices are the same size.
// Also, returns false if any of the pointers is NULL.
bool s21_are_the_same_size(matrix_t *A, matrix_t *B);

#endif  // S21_MATRIX_SUPPLEMENTARY_H_
