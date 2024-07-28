#include <stdlib.h>

#include "matrix.h"
#include "algorithms.h"

Matrix square_matrix_multiply(Matrix *left, Matrix *right) {
    size_t dimension = left->dimension;
    float *new_matrix_data = (float*)malloc(sizeof(float) * dimension * dimension);

    for (int i = 0; i < dimension; ++i)
        for (int j = 0; j < dimension; ++j) {
            new_matrix_data[i * dimension + j] = 0;
            for (int k = 0; k < dimension; ++k)
                new_matrix_data[i * dimension + j] += matrix_at(left, i, k) * matrix_at(right, k, j);
        }
            
    return (Matrix){ dimension, new_matrix_data };
}
