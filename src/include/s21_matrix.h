#ifndef S21_MATRIX_H_
#define S21_MATRIX_H_

enum return_codes {
  RC_OK = 0,
  RC_INCORRECT_MATRIX_PARAMETERS = 1,
  RC_INCORRECT_MATRIX = 1,
  RC_CALCULATIONS_CANNOT_BE_PERFORMED = 2,
  RC_MEMORY_ALLOCATION_FAILED = 3,
  RC_NULL_POINTER_INPUT = 4
};

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

/* creating/cleaninng */

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);

/* comparison */

int s21_eq_matrix(matrix_t *A, matrix_t *B);

/* arithmetics */
/** addition/subtraction */

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/** multiplicaiton */

int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/* operations unique to matrices */

int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

#endif  // S21_MATRIX_H_
