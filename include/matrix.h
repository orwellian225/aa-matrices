#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	size_t dimension;
	float *data;
} Matrix;

Matrix random_matrix(size_t dimension, float start, float end);
Matrix uniform_matrix(size_t dimension, float value);
Matrix identity_matrix(size_t dimension);

void clean_matrix(Matrix *m);

float matrix_at(Matrix *m, size_t row, size_t col);
float submatrix_at(Matrix *m, size_t start, size_t row, size_t col);

void submatrix_add(
    Matrix *result, Matrix *left, Matrix *right,
    size_t result_start, size_t left_start, size_t right_start,
    size_t sub_dimension
); 
void submatrix_subtract(
    Matrix *result, Matrix *left, Matrix *right,
    size_t result_start, size_t left_start, size_t right_start,
    size_t sub_dimension
); 

bool equal_matrices(Matrix *left, Matrix *right);

void print_matrix(Matrix *m);
void print_submatrix(Matrix *m, size_t start, size_t sub_dimension);

void pad_matrix(Matrix *m);
void depad_matrix(Matrix *m, size_t original_size);
