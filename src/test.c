#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "matrix.h"
#include "algorithms.h"

int main(int argc, char **argv) {
    size_t n = 2;    

    if (argc == 2) {
        n = atoi(argv[1]);
    }

    srand(time(NULL));

    /*Matrix m1 = uniform_matrix(n, 3.);*/
    /*Matrix m2 = uniform_matrix(n, 2.);*/
    Matrix m1 = random_matrix(n, -1., 1.);
    Matrix m2 = random_matrix(n, -1., 1.);
    /*Matrix m1 = identity_matrix(n);*/
    /*Matrix m2 = identity_matrix(n);*/
    Matrix result_smmr = uniform_matrix(n, 0.);
    Matrix result_strassens = uniform_matrix(n, 0.);

    /*float *m2_data = (float*)malloc(4 * sizeof(float));*/
    /*data[0] = 0.; data[1] = 0.; data[2] = 1.; data[3] = 0.;*/
    /*Matrix m2 = { 2, m2_data };*/

    pad_matrix(&m1);
    pad_matrix(&m2);
    pad_matrix(&result_smmr);
    pad_matrix(&result_strassens);

    printf("Matrix 1\n");
    print_matrix(&m1);
    printf("\n");

    printf("Matrix 2\n");
    print_matrix(&m2);
    printf("\n");

    square_matrix_multiply_recursive(
        &result_smmr, &m1, &m2, 0, 0, 0, result_smmr.dimension
    );
    strassens_method(&result_strassens, &m1, &m2, 0, 0, 0, result_strassens.dimension);
    Matrix result_smm = square_matrix_multiply(&m1, &m2);

    depad_matrix(&m1, n);
    depad_matrix(&m2, n);
    depad_matrix(&result_smm, n);
    depad_matrix(&result_smmr, n);
    depad_matrix(&result_strassens, n);

    printf("Matrix SMM Result\n");
    print_matrix(&result_smm);
    printf("\n");

    printf("Matrix SMMR Result\n");
    print_matrix(&result_smmr);
    printf("\n");

    printf("Matrix Strassens Result\n");
    print_matrix(&result_strassens);
    printf("\n");

    printf("SMM = SMMR matrices match: %b\n", equal_matrices(&result_smm, &result_smmr));
    printf("SMM = Strassens matrices match: %b\n", equal_matrices(&result_smm, &result_strassens));

    clean_matrix(&m1);
    clean_matrix(&m2);
    clean_matrix(&result_smmr);
    clean_matrix(&result_smm);
    clean_matrix(&result_strassens);
}
