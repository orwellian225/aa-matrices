#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void square_matrix_multiply_recursive(
    Matrix *result, Matrix *left, Matrix *right,
    size_t result_start, size_t left_start, size_t right_start, 
    size_t current_n
) {
    if (current_n == 1) {
        result->data[result_start] += 
            (submatrix_at(left, left_start, 0, 0) * 
            submatrix_at(right, right_start, 0, 0));
        return;
    }

    size_t start_C11 = result_start;
    size_t start_C12 = result_start + current_n / 2;
    size_t start_C21 = result_start + current_n * result->dimension / 2;
    size_t start_C22 = result_start + current_n * result->dimension / 2 + current_n / 2;

    size_t start_A11 = left_start;
    size_t start_A12 = left_start + current_n / 2;
    size_t start_A21 = left_start + current_n * left->dimension / 2;
    size_t start_A22 = left_start + current_n * left->dimension / 2 + current_n / 2;

    size_t start_B11 = right_start;
    size_t start_B12 = right_start + current_n / 2;
    size_t start_B21 = right_start + current_n * right->dimension / 2;
    size_t start_B22 = right_start + current_n * right->dimension / 2 + current_n / 2;

    square_matrix_multiply_recursive(
        result, left, right,
        start_C11, start_A11, start_B11,
        current_n / 2
    );
    square_matrix_multiply_recursive(
        result, left, right,
        start_C11, start_A12, start_B21,
        current_n / 2
    );

    square_matrix_multiply_recursive(
        result, left, right,
        start_C12, start_A11, start_B12,
        current_n / 2
    );
    square_matrix_multiply_recursive(
        result, left, right,
        start_C12, start_A12, start_B22,
        current_n / 2
    );

    square_matrix_multiply_recursive(
        result, left, right,
        start_C21, start_A21, start_B11,
        current_n / 2
    );
    square_matrix_multiply_recursive(
        result, left, right,
        start_C21, start_A22, start_B21,
        current_n / 2
    );

    square_matrix_multiply_recursive(
        result, left, right,
        start_C22, start_A21, start_B12,
        current_n / 2
    );
    square_matrix_multiply_recursive(
        result, left, right,
        start_C22, start_A22, start_B22,
        current_n / 2
    );

}

void strassens_method(
    Matrix *result, Matrix *left, Matrix *right,
    size_t result_start, size_t left_start, size_t right_start,
    size_t current_n
) {
    if (current_n == 1) {
        result->data[result_start] += 
            (submatrix_at(left, left_start, 0, 0) * 
            submatrix_at(right, right_start, 0, 0));
        return;
    }

    size_t start_C11 = result_start;
    size_t start_C12 = result_start + current_n / 2;
    size_t start_C21 = result_start + current_n * result->dimension / 2;
    size_t start_C22 = result_start + current_n * result->dimension / 2 + current_n / 2;

    size_t start_A11 = left_start;
    size_t start_A12 = left_start + current_n / 2;
    size_t start_A21 = left_start + current_n * left->dimension / 2;
    size_t start_A22 = left_start + current_n * left->dimension / 2 + current_n / 2;

    size_t start_B11 = right_start;
    size_t start_B12 = right_start + current_n / 2;
    size_t start_B21 = right_start + current_n * right->dimension / 2;
    size_t start_B22 = right_start + current_n * right->dimension / 2 + current_n / 2;

    // I'm not really sure how to be doing this in-place with global S and P matrices,
    // So instead as an optimisation I'll be recycling the heap memory as much as possible
    //
    // !<> S1 -- cannot recycle anything for S1 so it must be initialized
    // S1 can be recycled as (<>) S2
    // S2 <> S3
    // S3 <> S4
    // S4 <> S5
    // !<> S6
    // S5 <> S7
    // S6 <> S8
    // S7 <> S9
    // S8 <> S10
    //
    // !<> P1
    // !<> P2
    // !<> P3
    // !<> P4
    // !<> P5
    // !<> P6
    // P6 <> P7 -- P6 is last used before P7 must be initialized
    
    // Need at least 8 mem-buffers
    float *mem_buffers[8] = {
        (float*) malloc(sizeof(float) * current_n * current_n / 4),
        (float*) malloc(sizeof(float) * current_n * current_n / 4),
        (float*) malloc(sizeof(float) * current_n * current_n / 4),
        (float*) malloc(sizeof(float) * current_n * current_n / 4),
        (float*) malloc(sizeof(float) * current_n * current_n / 4),
        (float*) malloc(sizeof(float) * current_n * current_n / 4),
        (float*) malloc(sizeof(float) * current_n * current_n / 4),
        (float*) malloc(sizeof(float) * current_n * current_n / 4),
    };

    Matrix S1 = { current_n / 2, mem_buffers[0] };
    Matrix S2 = { current_n / 2, mem_buffers[0] };
    Matrix S3 = { current_n / 2, mem_buffers[0] };
    Matrix S4 = { current_n / 2, mem_buffers[0] };
    Matrix S5 = { current_n / 2, mem_buffers[0] };
    Matrix S6 = { current_n / 2, mem_buffers[1] };
    Matrix S7 = { current_n / 2, mem_buffers[0] };
    Matrix S8 = { current_n / 2, mem_buffers[1] };
    Matrix S9 = { current_n / 2, mem_buffers[0] };
    Matrix S10 = { current_n / 2, mem_buffers[1] };

    // Do need to memset each of these
    Matrix P1 = { current_n / 2, mem_buffers[2] };
    Matrix P2 = { current_n / 2, mem_buffers[3] };
    Matrix P3 = { current_n / 2, mem_buffers[4] };
    Matrix P4 = { current_n / 2, mem_buffers[5] };
    Matrix P5 = { current_n / 2, mem_buffers[6] };
    Matrix P6 = { current_n / 2, mem_buffers[7] };
    Matrix P7 = { current_n / 2, mem_buffers[7] };

    for (size_t i = 2; i < 8; ++i)
        memset(mem_buffers[i], 0, sizeof(float) * current_n * current_n / 4);

    /*printf("A11\n"); print_submatrix(left, start_A11, current_n / 2); printf("\n");*/
    /*printf("A12\n"); print_submatrix(left, start_A12, current_n / 2); printf("\n");*/
    /*printf("A21\n"); print_submatrix(left, start_A21, current_n / 2); printf("\n");*/
    /*printf("A22\n"); print_submatrix(left, start_A22, current_n / 2); printf("\n");*/

    /*printf("B11\n"); print_submatrix(right, start_B11, current_n / 2); printf("\n");*/
    /*printf("B12\n"); print_submatrix(right, start_B12, current_n / 2); printf("\n");*/
    /*printf("B21\n"); print_submatrix(right, start_B21, current_n / 2); printf("\n");*/
    /*printf("B22\n"); print_submatrix(right, start_B22, current_n / 2); printf("\n");*/

    // Don't need to memset because addition / subtraction overrides the original data
    submatrix_subtract(&S1, right, right, 0, start_B12, start_B22, current_n / 2);
    /*printf("S1\n"); print_matrix(&S1); printf("\n");*/
    strassens_method(&P1, left, &S1, 0, start_A11, 0, current_n / 2);
    /*printf("P1\n"); print_matrix(&P1); printf("\n");*/

    submatrix_add(&S2, left, left, 0, start_A11, start_A12, current_n / 2);
    /*printf("S2\n"); print_matrix(&S2); printf("\n");*/
    strassens_method(&P2, &S2, right, 0, 0, start_B22, current_n / 2);
    /*printf("P2\n"); print_matrix(&P2); printf("\n");*/

    submatrix_add(&S3, left, left, 0, start_A21, start_A22, current_n / 2);
    /*printf("S3\n"); print_matrix(&S3); printf("\n");*/
    strassens_method(&P3, &S3, right, 0, 0, start_B11, current_n / 2);
    /*printf("P3\n"); print_matrix(&P3); printf("\n");*/

    submatrix_subtract(&S4, right, right, 0, start_B21, start_B11, current_n / 2);
    /*printf("S4\n"); print_matrix(&S4); printf("\n");*/
    strassens_method(&P4, left, &S4, 0, start_A22, 0, current_n / 2);
    /*printf("P4\n"); print_matrix(&P4); printf("\n");*/

    submatrix_add(&S5, left, left, 0, start_A11, start_A22, current_n / 2);
    /*printf("S5\n"); print_matrix(&S5); printf("\n");*/
    submatrix_add(&S6, right, right, 0, start_B11, start_B22, current_n / 2);
    /*printf("S6\n"); print_matrix(&S6); printf("\n");*/
    strassens_method(&P5, &S5, &S6, 0, 0, 0, current_n / 2);
    /*printf("P5\n"); print_matrix(&P5); printf("\n");*/

    submatrix_subtract(&S7, left, left, 0, start_A12, start_A22, current_n / 2);
    /*printf("S7\n"); print_matrix(&S7); printf("\n");*/
    submatrix_add(&S8, right, right, 0, start_B21, start_B22, current_n / 2);
    /*printf("S8\n"); print_matrix(&S8); printf("\n");*/
    strassens_method(&P6, &S7, &S8, 0, 0, 0, current_n / 2);
    /*printf("P6\n"); print_matrix(&P6); printf("\n");*/

    // C11 - done before P7 for recycling
    submatrix_subtract(result, result, &P2, start_C11, start_C11, 0, current_n / 2);
    submatrix_add(result, result, &P4, start_C11, start_C11, 0, current_n / 2);
    submatrix_add(result, result, &P5, start_C11, start_C11, 0, current_n / 2);
    submatrix_add(result, result, &P6, start_C11, start_C11, 0, current_n / 2);
    /*printf("C11\n"); print_submatrix(result, start_C11, current_n / 2); printf("\n");*/

    // Reseting for recycle
    memset(mem_buffers[7], 0, sizeof(float) * current_n * current_n / 4);
    submatrix_subtract(&S9, left, left, 0, start_A11, start_A21, current_n / 2);
    /*printf("S9\n"); print_matrix(&S9); printf("\n");*/
    submatrix_add(&S10, right, right, 0, start_B11, start_B12, current_n / 2);
    /*printf("S10\n"); print_matrix(&S10); printf("\n");*/
    strassens_method(&P7, &S9, &S10, 0, 0, 0, current_n / 2);
    /*printf("P7\n"); print_matrix(&P7); printf("\n");*/

    // C12
    submatrix_add(result, result, &P1, start_C12, start_C12, 0, current_n / 2);
    submatrix_add(result, result, &P2, start_C12, start_C12, 0, current_n / 2);
    /*printf("C12\n"); print_submatrix(result, start_C12, current_n / 2); printf("\n");*/

    // C21
    submatrix_add(result, result, &P3, start_C21, start_C21, 0, current_n / 2);
    submatrix_add(result, result, &P4, start_C21, start_C21, 0, current_n / 2);
    /*printf("C21\n"); print_submatrix(result, start_C21, current_n / 2); printf("\n");*/

    // C22
    submatrix_add(result, result, &P1, start_C22, start_C22, 0, current_n / 2);
    submatrix_subtract(result, result, &P3, start_C22, start_C22, 0, current_n / 2);
    submatrix_add(result, result, &P5, start_C22, start_C22, 0, current_n / 2);
    submatrix_subtract(result, result, &P7, start_C22, start_C22, 0, current_n / 2);
    /*printf("C22\n"); print_submatrix(result, start_C22, current_n / 2); printf("\n");*/

    for (size_t i = 0; i < 8; ++i)
        free(mem_buffers[i]);
}
