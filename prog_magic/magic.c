#include "header.h"

int main()
{
    /* Define variables: */
    matrix mat;
    int count = 0; /* Counts the number of integers inside the matrix */
    int valid = 1; /* vaild: defines 1 if there are less than N^2 values and 0 if more than N^2 values */
    char input;
    int i = 0, j = 0;

    printf("Please Enter numbers between 1 - %d for every line\n and up to %d lines:\n\n", (N*N), N);

    while ((input = getchar()) != EOF)
    /* input values from the user */
    {
        if (isdigit(input) == 0 && isspace(input) == 0) /* checks that the value is an integer */
        {
            printf("\nError: variable is not a number, You have to input numbers ONLY\n");
            return -1;
        }
        else if (isspace(input) == 0)
        {
            if(valid == 0)
            {
                printf("\nError: Too many values!!\n");
                return -1;
            }
            mat[i][j] = (input - '0');
            count++;
            if (j == (N - 1)) /* True: to the next Line in matrix */
            {
                j = -1;
                i++;
            }
            if (i > (N - 1)) /* True: there are more than N^2 values */
            {
                valid = 0;
            }
            j++;
        }
    }

    if (count < (N*N)) /* returns if there are not enough values */
    {
        printf("\nError:  Not enough values :(\n");
        return -1;
    }
    printMatrix(mat);     /* print Matrix */
    if (magicM(mat) == 1) /* returns if matrix is magical */
    {
        printf("\n* \t *\t *\t *\t *");
        printf("\n\t The Matrix is magical !!\n");
        printf("* \t *\t *\t *\t *\n");
    }
    else
    {
        printf("\n\n\t The Matrix is NOT magical \n");
    }
    return 0;
}

int magicM(matrix mat) /* method: Checks if the matrix is magical */
{
    /* Define variables */
    int i;
    int sumLines = 0;
    int sumColumns = 0;
    int sumDiagonal = 0;
    if (diagonal(mat) != 1 || lines(mat) != 1 || columns(mat) != 1 || inRange(mat) != 1)
    {
        return 0;
    }

    for (i = 0; i < N; i++)
    {
        sumLines += mat[i][0];
        sumColumns += mat[0][i];
        sumDiagonal += mat[i][i];
    }
    if (sumLines == sumColumns && sumDiagonal == sumColumns)
    {
        return 1;
    }
    return 0;
}
void printMatrix(matrix mat) /* method: Prints the matrix */
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (j == 0)
            {
                printf("%d\t", mat[i][j]);
            }
            else
            {
                printf("%d\t", mat[i][j]);
            }
        }
        printf("\n");
    }
}
int inRange(matrix mat) /* method: Checks that the matrix values in the range between 1 and N^2 */
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (mat[i][j] > (N * N) || mat[i][j] < 1)
                return 0;
        }
    }
    return 1;
}
int diagonal(matrix mat) /* method: Checks if the diagonal lengths are equal to each other */
{
    int currentSum = 0;
    int sum = 0;
    int i, j = N - 1;
    for (i = 0; i < N; i++, j--)
    {
        sum += mat[i][i];
        currentSum += mat[i][j];
    }
    if (sum == currentSum)
    {
        return 1;
    }
    return 0;
}
int lines(matrix mat) /*  method: Checks if the line lengths are equal to each other */
{
    int currentSum = 0;
    int sum = 0;
    int firstLine = 1;
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (firstLine == 1)
            {
                sum += mat[i][j];
            }
            else
            {
                currentSum += mat[i][j];
            }
        }
        if (firstLine == 0)
        {
            if (sum != currentSum)
            {
                return 0;
            }
        }
        currentSum = 0;
        firstLine = 0;
    }
    return 1;
}
int columns(matrix mat) /*  method: Checks if the column lengths are equal to each other */
{
    int currentSum = 0;
    int sum = 0;
    int firstCol = 1;
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (firstCol == 1)
            {
                sum += mat[j][i];
            }
            else
            {
                currentSum += mat[j][i];
            }
        }
        if (firstCol == 0)
        {
            if (sum != currentSum)
            {
                return 0;
            }
        }
        currentSum = 0;
        firstCol = 0;
    }
    return 1;
}