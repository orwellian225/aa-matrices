#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

#include "matrix.h"

float matrix_at(Matrix *m, size_t row, size_t col) {
    return m->data[row * m->dimension + col];
}

float submatrix_at(Matrix *m, size_t start, size_t row, size_t col) {
    return m->data[start +  row * m->dimension + col];
}

void submatrix_add(
    Matrix *result, Matrix *left, Matrix *right,
    size_t result_start, size_t left_start, size_t right_start,
    size_t sub_dimension
) {
    for (int row = 0; row < sub_dimension; ++row) {
        for (int col = 0; col < sub_dimension; ++col) {
            result->data[result_start + row * result->dimension + col] = submatrix_at(left, left_start, row, col) + submatrix_at(right, right_start, row, col);
        }
    }
}

void submatrix_subtract(
    Matrix *result, Matrix *left, Matrix *right,
    size_t result_start, size_t left_start, size_t right_start,
    size_t sub_dimension
) {
    for (int row = 0; row < sub_dimension; ++row) {
        for (int col = 0; col < sub_dimension; ++col) {
            result->data[result_start + row * result->dimension + col] = submatrix_at(left, left_start, row, col) - submatrix_at(right, right_start, row, col);
        }
    }
}

void pad_matrix(Matrix *m) {
    size_t new_size = (size_t)(pow(2, ceil(log2(m->dimension))));

    if (new_size == m->dimension)
        return;

    float *new_data = (float*)malloc(sizeof(float) * new_size * new_size);
    memset(new_data, 0., new_size * new_size * sizeof(float));

    // a b c d e f g h -> a b c 0 d e f 0 g h 0 0 0 0 0
    for (size_t i = 0; i < m->dimension; ++i)
        for (size_t j = 0; j < m->dimension; ++j)
            new_data[i * new_size + j] = m->data[i * m->dimension + j];

    free(m->data);
    m->data = new_data;
    m->dimension = new_size;
}

void depad_matrix(Matrix *m, size_t original_size) {
    if (original_size == m->dimension)
        return;

    float *new_data = (float*)malloc(sizeof(float) * original_size * original_size);
    for (size_t i = 0; i < original_size; ++i)
        for (size_t j = 0; j < original_size; ++j)
            new_data[i * original_size + j] = m->data[i * m->dimension + j];

    free(m->data);
    m->data = new_data;
    m->dimension = original_size;
}

bool equal_matrices(Matrix *left, Matrix *right) {
    if (left->dimension != right->dimension)
        return false;

    for (size_t i = 0; i < left->dimension; ++i)
        for (size_t j = 0; j < left->dimension; ++j) {
            if (fabs(matrix_at(left, i, j) - matrix_at(right, i, j)) > 0.1)
                return false;
        }

    return true;
}

Matrix random_matrix(size_t dimension, float start, float end) {
    float *data = (float*)malloc(dimension * dimension * sizeof(float));

    for (size_t i = 0; i < dimension * dimension; ++i) {
        data[i] = start +  rand() / (float)RAND_MAX * (end - start);
    }

    return (Matrix) {
        dimension,
        data
    };
}

Matrix uniform_matrix(size_t dimension, float value) {
    float *data = (float*)malloc(dimension * dimension * sizeof(float));

    for (size_t i = 0; i < dimension * dimension; ++i) {
        data[i] = value;
    }

    return (Matrix) {
        dimension,
        data
    };
}

Matrix identity_matrix(size_t dimension) {
    float *data = (float*)malloc(dimension * dimension * sizeof(float));
    memset(data, 0., dimension * dimension * sizeof(float));

    for (size_t i = 0; i < dimension * dimension; i += dimension + 1) {
        data[i] = 1.;
    }

    return (Matrix) {
        dimension,
        data
    };
}

void print_matrix(Matrix *m) {
    for (int row = 0; row < m->dimension; ++row) {
        for (int col = 0; col < m->dimension; ++col) {
            printf("%f ", matrix_at(m, row, col));
        }
        printf("\n");
    }
}

void print_submatrix(Matrix *m, size_t start, size_t sub_dimension) {
    for (int row = 0; row < sub_dimension; ++row) {
        for (int col = 0; col < sub_dimension; ++col) {
            printf("%f ", submatrix_at(m, start, row, col));
        }
        printf("\n");
    }
}

void clean_matrix(Matrix *m) {
    free(m->data);
}
