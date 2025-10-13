// Code originally made by Tonie 
// Used for lessons on arrays and matrices in C
// This code demonstrates basic 1D and 2D array usage in C

// libraries
#include <stdio.h>

int main() {
    // ------------------ 1D ARRAY SECTION ------------------
    int numbers[5]; // array declaration with 5 integer elements
    int i;          // loop counter variable

    // input elements into the array
    printf("=== 1D ARRAY EXAMPLE ===\n");
    printf("Enter 5 numbers:\n");
    for (i = 0; i < 5; i++) {
        printf("Number %d: ", i + 1);
        scanf("%d", &numbers[i]); // store user input in array
        // & is used to get the address of the array element
    }

    // output the elements of the array
    printf("\nYou entered:\n");
    for (i = 0; i < 5; i++) {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }

    // show the sum of all numbers in the array
    int sum = 0;
    for (i = 0; i < 5; i++) {
        sum += numbers[i]; // add each element to sum
    }

    printf("\nSum of all numbers: %d\n", sum);


    // ------------------ 2D ARRAY (MATRIX) SECTION ------------------
    int matrix[2][3]; // 2 rows, 3 columns
    int j;            // loop counter for columns

    // input elements for the matrix
    printf("\n=== 2D ARRAY (MATRIX) EXAMPLE ===\n");
    printf("Enter elements for a 2x3 matrix:\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            scanf("%d", &matrix[i][j]);
            // & gets the address of each matrix element
        }
    }

    // display the matrix
    printf("\nMatrix entered:\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n"); // move to next row
    }

    // calculate and display sum of all elements
    int matrixSum = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            matrixSum += matrix[i][j]; // add each element
        }
    }

    printf("\nSum of all elements in the matrix: %d\n", matrixSum);

    return 0;
}
