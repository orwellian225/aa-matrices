#pragma once

#include "matrix.h"

Matrix square_matrix_multiply(Matrix *left, Matrix *right);
void square_matrix_multiply_recursive(
    Matrix *result, Matrix *left, Matrix *right,
    size_t result_start, size_t left_start, size_t right_start, 
    size_t current_n
);
void strassens_method(
        Matrix *result, Matrix *left, Matrix *right,
        size_t result_start, size_t left_start, size_t right_start,
        size_t current_n
);

