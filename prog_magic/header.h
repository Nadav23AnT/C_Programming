#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> /*  */

#define N 5 /* defines the length of the matrix */

typedef int matrix[N][N]; /* type integers into matrix size N * N */

int magicM(matrix mat); /* method: Checks if the matrix is magical */
void printMatrix(matrix mat); /* method: Prints the matrix */
int inRange(matrix mat); /* method: Checks that the matrix values in the range between 1 and N^2 */
int diagonal(matrix mat); /* method: Checks if the diagonal lengths are equal to each other*/
int lines(matrix mat); /* method: Checks if the line lengths are equal to each other*/
int columns(matrix mat); /* method: Checks if the column lengths are equal to each other*/