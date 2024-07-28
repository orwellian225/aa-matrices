#pragma once

#include <stddef.h>

typedef struct Matrix {
	size_t dimension;
	float *data;

} Matrix;

Matrix random_matrix(size_t dimension, float start, float end);
Matrix uniform_matrix(size_t dimension, float value);
void clean_matrix(Matrix *m);

float matrix_at(Matrix *m, size_t row, size_t col);
void print_matrix(Matrix *m);
