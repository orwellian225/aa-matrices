#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "matrix.h"

float matrix_at(Matrix *m, size_t row, size_t col) {
    return m->data[row * m->dimension + col];
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

void print_matrix(Matrix *m) {
    for (int row = 0; row < m->dimension; ++row) {
        for (int col = 0; col < m->dimension; ++col) {
            printf("%f ", matrix_at(m, row, col));
        }
        printf("\n");
    }
}

void clean_matrix(Matrix *m) {
    free(m->data);
}
