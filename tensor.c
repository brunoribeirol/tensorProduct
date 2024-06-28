#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ROWS 100
#define MAX_COLUMNS 100
#define THREADS 2

typedef struct Matrix
{
    int matrix[MAX_ROWS][MAX_COLUMNS];
    int rows;
    int columns;
} Matrix;

typedef struct MatrixTensorProduct
{
    int tensor[MAX_ROWS * MAX_ROWS][MAX_COLUMNS * MAX_COLUMNS];
    int rowsTensor;
    int columnsTensor;
} MatrixTensorProduct;

Matrix matrix1;
Matrix matrix2;
MatrixTensorProduct matrixTensorProduct;
pthread_t threads[THREADS];
int threadID[THREADS];

int matrixRows(FILE *file);
int matrixColumns(FILE *file);
void matrixData(Matrix *matrix, FILE *file);
void tensorProduct();
void *computeTensorPart(void *arg);
void writeOutputFile(MatrixTensorProduct *matrixTensorProduct, FILE *file);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Error: Expected exactly 2 matrix files as arguments.\n");
        fprintf(stderr, "Usage: %s <matrix_file1> <matrix_file2>\n", argv[0]);
        return 1;
    }

    FILE *inputFile1 = fopen(argv[1], "r");
    FILE *inputFile2 = fopen(argv[2], "r");

    FILE *outputFile = fopen("tensor.out", "w");

    if (inputFile1 == NULL || inputFile2 == NULL || outputFile == NULL)
    {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    matrix1.rows = matrixRows(inputFile1);
    matrix1.columns = matrixColumns(inputFile1);
    matrixData(&matrix1, inputFile1);

    matrix2.rows = matrixRows(inputFile2);
    matrix2.columns = matrixColumns(inputFile2);
    matrixData(&matrix2, inputFile2);

    fclose(inputFile1);
    fclose(inputFile2);

    tensorProduct();
    writeOutputFile(&matrixTensorProduct, outputFile);

    fclose(outputFile);

    return 0;
}

int matrixRows(FILE *file)
{
    char ch;
    int rows = 0;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            rows++;
        }
    }

    if (ch != '\n' && rows > 0)
    {
        rows++;
    }
    rewind(file);

    return rows;
}

int matrixColumns(FILE *file)
{
    char ch;
    int columns = 0;

    while ((ch = fgetc(file)) != EOF && ch != '\n')
    {
        if (ch == ' ' || ch == '\t')
        {
            columns++;
        }
    }
    rewind(file);

    return columns + 1;
}

void matrixData(Matrix *matrix, FILE *file)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->columns; j++)
        {
            if (fscanf(file, "%d", &matrix->matrix[i][j]) != 1)
            {
                fprintf(stderr, "Error reading matrix element at (%d, %d)\n", i, j);
                exit(EXIT_FAILURE);
            }
        }
    }
    rewind(file);
}

void tensorProduct()
{
    matrixTensorProduct.rowsTensor = matrix1.rows * matrix2.rows;
    matrixTensorProduct.columnsTensor = matrix1.columns * matrix2.columns;

    for (int i = 0; i < THREADS; i++)
    {
        threadID[i] = i;
        pthread_create(&threads[i], NULL, computeTensorPart, &threadID[i]);
    }

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void *computeTensorPart(void *arg)
{
    int tid = *(int *)arg;

    for (int i = tid; i < matrixTensorProduct.rowsTensor; i += THREADS)
    {
        for (int j = 0; j < matrixTensorProduct.columnsTensor; j++)
        {
            int matrix1Row = i / matrix2.rows;
            int matrix1Column = j / matrix2.columns;
            int matrix2Row = i % matrix2.rows;
            int matrix2Column = j % matrix2.columns;

            matrixTensorProduct.tensor[i][j] = matrix1.matrix[matrix1Row][matrix1Column] * matrix2.matrix[matrix2Row][matrix2Column];
        }
    }
    pthread_exit(NULL);
}

void writeOutputFile(MatrixTensorProduct *matrixTensorProduct, FILE *file)
{
    int maxColumnWidth[matrixTensorProduct->columnsTensor];

    for (int j = 0; j < matrixTensorProduct->columnsTensor; j++)
    {
        maxColumnWidth[j] = 0;
        for (int i = 0; i < matrixTensorProduct->rowsTensor; i++)
        {
            int number = matrixTensorProduct->tensor[i][j];
            int width = 1;

            if (number < 0)
            {
                width++;
                number = -number;
            }

            while (number >= 10)
            {
                width++;
                number /= 10;
            }

            if (width > maxColumnWidth[j])
            {
                maxColumnWidth[j] = width;
            }
        }
    }

    for (int i = 0; i < matrixTensorProduct->rowsTensor; i++)
    {
        for (int j = 0; j < matrixTensorProduct->columnsTensor; j++)
        {
            fprintf(file, "%*d", maxColumnWidth[j], matrixTensorProduct->tensor[i][j]);
            if (j < matrixTensorProduct->columnsTensor - 1)
            {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }
}