#include <stdio.h>
#include <time.h>

#include "matrix.h"
#include "algorithms.h"

int main(int argc, char **argv) {
    FILE *csv_out = fopen("../data/results.csv", "w");
    fprintf(csv_out, "n,smm_duration_s,smmr_duration_s,sm_duration_s");
    printf("n, smm_duration_s, smmr_duration_s, sm_duration_s\n");

    clock_t start_time, end_time;
    double smm_duration, smmr_duration, sm_duration;
    Matrix smm_matrix, smmr_matrix, sm_matrix;
    Matrix left_matrix, right_matrix;
    for (size_t n = 1; n <= 10000; n *= 10) {
        left_matrix = random_matrix(n, -10., 10.);
        right_matrix = random_matrix(n, -10., 10.);

        start_time = clock();
            smm_matrix = square_matrix_multiply(&left_matrix, &right_matrix);
        end_time = clock();
        smm_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        start_time = clock();
        /*    smmr_matrix = square_matrix_multiply_recursive(&left_matrix, &right_matrix);*/
        end_time = clock();
        smmr_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        start_time = clock();
        /*    sm_matrix = strassens_method(&left_matrix, &right_matrix);*/
        end_time = clock();
        sm_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        printf("%ld, %lf, %lf, %lf\n", n, smm_duration, smmr_duration, sm_duration);
        fprintf(csv_out, "%ld,%lf,%lf,%lf\n", n, smm_duration, smmr_duration, sm_duration);
    }

    fclose(csv_out);

	return 0;
}
