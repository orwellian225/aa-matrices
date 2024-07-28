#pragma once

#include "matrix.h"

Matrix square_matrix_multiply(Matrix *left, Matrix *right);
Matrix square_matrix_multiply_recursive(Matrix *left, Matrix *right);
Matrix strassens_method(Matrix *left, Matrix *right);
