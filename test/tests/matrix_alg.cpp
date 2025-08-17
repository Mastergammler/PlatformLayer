#include "../../src/rendering/module/matrix.cpp"
#include "../imports.h"

void matrix_print(Matrix<int> m)
{
    log("");
    string row;

    for (int r = 0; r < m.rows; r++)
    {
        row = "           [";
        char tmp[16];
        for (int c = 0; c < m.columns; c++)
        {
            sprintf(tmp, " %2d ", m(r, c));
            row += tmp;
        }
        row += "]";
        logf(row, m.data + r * m.columns);
    }
}

// TODO: doesn't handle sameness check
int matrix_diff(Matrix<int> m1, Matrix<int> m2)
{
    int diffCount = 0;
    /*for (int r = 0; r < m1.rows; r++)
    {
        for (int c = 0; c < m1.columns; c++)
        {
            if (m1(r, c) != m2(r, c)) diffCount++;
        }
    }*/

    for (int i = 0; i < m1.cell_count; i++)
    {
        if (m1.data[i] != m2.data[i]) diffCount++;
    }

    if (diffCount > 0) matrix_print(m1);

    return diffCount;
}

Matrix<int> matrix_create(int rows, int columns, int* data)
{
    Matrix<int> m = {};
    m.columns = columns;
    m.rows = rows;
    m.cell_count = m.columns * m.rows;
    m.data = data;

    return m;
}

int row_swap_4x3()
{
    int srcData[12] = {1, 5, 9, 2, 6, 10, 3, 7, 11, 4, 8, 12};
    int expData[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Matrix<int> m = matrix_create(4, 3, srcData);
    Matrix<int> exp = matrix_create(4, 3, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}

int row_swap_6x4()
{
    int srcData[24] = {1, 7,  13, 19, 2, 8,  14, 20, 3, 9,  15, 21,
                       4, 10, 16, 22, 5, 11, 17, 23, 6, 12, 18, 24};
    int expData[24] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
                       13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    Matrix<int> m = matrix_create(6, 4, srcData);
    Matrix<int> exp = matrix_create(6, 4, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}

int row_swap_3x3()
{
    int srcData[9] = {1, 4, 7, 2, 5, 8, 3, 6, 9};
    int expData[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Matrix<int> m = matrix_create(3, 3, srcData);
    Matrix<int> exp = matrix_create(3, 3, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}

int row_swap_1x3()
{
    int srcData[3] = {1, 2, 3};
    int expData[3] = {1, 2, 3};
    Matrix<int> m = matrix_create(1, 3, srcData);
    Matrix<int> exp = matrix_create(1, 3, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}

int row_swap_1x1()
{
    int srcData[1] = {1};
    int expData[1] = {1};
    Matrix<int> m = matrix_create(1, 1, srcData);
    Matrix<int> exp = matrix_create(1, 1, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}

int row_swap_2x2()
{
    int srcData[4] = {1, 3, 2, 4};
    int expData[4] = {1, 2, 3, 4};
    Matrix<int> m = matrix_create(2, 2, srcData);
    Matrix<int> exp = matrix_create(2, 2, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}

int row_swap_3x1()
{
    int srcData[3] = {1, 2, 3};
    int expData[3] = {1, 2, 3};
    Matrix<int> m = matrix_create(3, 1, srcData);
    Matrix<int> exp = matrix_create(3, 1, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}

int row_swap_2x3()
{
    int srcData[6] = {1, 3, 5, 2, 4, 6};
    int expData[6] = {1, 2, 3, 4, 5, 6};
    Matrix<int> m = matrix_create(2, 3, srcData);
    Matrix<int> exp = matrix_create(2, 3, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}

int row_swap_3x2()
{
    int srcData[6] = {1, 4, 2, 5, 3, 6};
    int expData[6] = {1, 2, 3, 4, 5, 6};
    Matrix<int> m = matrix_create(3, 2, srcData);
    Matrix<int> exp = matrix_create(3, 2, expData);

    matrix_row_swap(m);

    return matrix_diff(m, exp);
}
