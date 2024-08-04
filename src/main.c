#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "matrix.h"
#include "algorithms.h"

int main(int argc, char **argv) {
    size_t max_n = 100;
    size_t step_n = 10;
    bool show_errors = false;
    if (argc == 4) {
        show_errors = (bool)atoi(argv[1]);
        max_n = atoi(argv[2]);
        step_n = atoi(argv[3]);
    }

    FILE *csv_out = fopen("../data/results.csv", "a");
    /*fprintf(csv_out, "n,smm_duration_s,smmr_duration_s,sm_duration_s\n");*/
    printf("n, smm_duration_s, smmr_duration_s, sm_duration_s\n");

    clock_t start_time, end_time;
    double smm_duration, smmr_duration, sm_duration;
    Matrix smm_matrix, smmr_matrix, sm_matrix;
    Matrix left_matrix, right_matrix;
    for (size_t n = step_n; n <= max_n; n += step_n) {
        left_matrix = random_matrix(n, -1., 1.);
        right_matrix = random_matrix(n, -1., 1.);

        start_time = clock();
            smm_matrix = square_matrix_multiply(&left_matrix, &right_matrix);
        end_time = clock();
        smm_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        start_time = clock();
            smmr_matrix = uniform_matrix(n, 0.);
            pad_matrix(&smmr_matrix);
            pad_matrix(&left_matrix);
            pad_matrix(&right_matrix);
            square_matrix_multiply_recursive(
                &smmr_matrix, &left_matrix, &right_matrix,
                0, 0, 0, smmr_matrix.dimension
            );
        end_time = clock();
        smmr_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        depad_matrix(&smmr_matrix, n);
        depad_matrix(&left_matrix, n);
        depad_matrix(&right_matrix, n);

        start_time = clock();
            sm_matrix = uniform_matrix(n, 0);
            pad_matrix(&sm_matrix);
            pad_matrix(&left_matrix);
            pad_matrix(&right_matrix);
            strassens_method(
                &sm_matrix, &left_matrix, &right_matrix,
                0, 0, 0, sm_matrix.dimension
            );
        end_time = clock();
        sm_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        depad_matrix(&sm_matrix, n);
        depad_matrix(&left_matrix, n);
        depad_matrix(&right_matrix, n);

        bool smmr_correct = equal_matrices(&smm_matrix, &smmr_matrix);
        bool strassen_correct = equal_matrices(&smm_matrix, &sm_matrix);

        printf("%ld, %lf, %lf, %lf, %b, %b\n", n, smm_duration, smmr_duration, sm_duration, smmr_correct, strassen_correct);
        fprintf(csv_out, "%ld, %lf, %lf, %lf, %b, %b\n", n, smm_duration, smmr_duration, sm_duration, smmr_correct, strassen_correct);

        if (!smmr_correct && show_errors) {
            printf("SMM\n");
            print_matrix(&smm_matrix);
            printf("\nSMMR\n");
            print_matrix(&smmr_matrix);
        }

        if (!strassen_correct && show_errors) {
            printf("SMM\n");
            print_matrix(&smm_matrix);
            printf("\nStrassen\n");
            print_matrix(&sm_matrix);
        }

        clean_matrix(&left_matrix);
        clean_matrix(&right_matrix);
        clean_matrix(&smm_matrix);
        clean_matrix(&smmr_matrix);
    }

    printf("\n");
    fclose(csv_out);

	return 0;
}
