//Класс объекта-матрицы

#include "MatrixClass.h"
#include <math.h>
#include <algorithm>
#include <iostream>

using namespace std;

void MatrixClass::FlushVariables()
{
    //printf("FlushVariables\n");
    nMatrix = 0;  //#Обнуление указателе на массивый. Если этого не сделать то при вызове delete в деструкторе будут ошибки
    wMatrix = 0;
    pMatrix = 0;
    rMatrix = 0;
    cMatrix = 0;
    bMatrix = 0;
    kMatrix = 0;
    uMatrix = 0;
    iMatrix = 0;
}

MatrixClass::MatrixClass()
{
    //printf("MatrixClass\n");
    FlushVariables();  //#Обнуление указателе на массивый. Если этого не сделать то при вызове delete в деструкторе будут ошибки
}

MatrixClass::MatrixClass(double *matrix_mas, int rows, int cols)
    : MatrixClass()
{
    //FlushVariables();  //#Обнуление указателе на массивый. Если этого не сделать то при вызове delete в деструкторе будут ошибки
    //printf("MatrixClass::MatrixClass(double *matrix_mas, int rows, int cols)\n");
    nMatrix = new double[rows*cols];

    for(int i = 0; i < rows*cols;i++)nMatrix[i] = matrix_mas[i]; //Формирование копии полученного массива
    //memcpy(nMatrix, matrix_mas, rows*cols*sizeof(double));     //Формирование копии полученного массива
    this->rows = rows;
    this->cols = cols;
    this->elements = rows*cols;

    //PrintMatrixData("MatrixClass::MatrixClass(double *matrix_mas, int rows, int cols)\n");
}

MatrixClass::MatrixClass(MatrixClass const* matrix, MatrixType type, int row, int col)
    : MatrixClass(matrix->nMatrix, matrix->rows, matrix->cols) //#Вызов другого конструктора и формирование видоизмененной матрицы
{
    //printf("MatrixClass::MatrixClass(MatrixClass const* matrix, MatrixType type, int row, int col)\n");

    switch(type){
    case MATRIX_TYPE_COPY:
        break;
    case MATRIX_TYPE_TRANSPOSE:
        Transpose();
        break;
    case MATRIX_TYPE_PMATRIX:
        PMatrix();
        break;
    case MATRIX_TYPE_WMATRIX:
        WMatrix();
        break;
    case MATRIX_TYPE_TONEURO:
        NeuroConvert();
        break;
    case MATRIX_TYPE_FROMNEURO:
        NeuroConvert();
        break;
    case MATRIX_TYPE_ROW:
        RowMatrix(row);
        break;
    case MATRIX_TYPE_COLUMN:
        ColMatrix(col);
        break;
    case MATRIX_TYPE_CROSSOUT:         //#Получение вычеркнутой матрицы
        CrossOutMatrix(row, col);
        break;
    case MATRIX_TYPE_UNION:
        UMatrix();
        break;
    case MATRIX_TYPE_INVERT:
        IMatrix();
        break;
    }
}

MatrixClass::MatrixClass(MatrixClass const& MatrixCopy)//#Конструктор копироваия
{
    FlushVariables();  //#Обнуление указателе на массивый. Если этого не сделать то при вызове delete в деструкторе будут ошибки

    rows     = MatrixCopy.rows;
    cols     = MatrixCopy.cols;
    elements = MatrixCopy.elements;

    printf("MatrixClass::MatrixClass(MatrixClass const& MatrixCopy)\n");

    //#Шаблоны для этого пока не проканали =(((
    //#Внимание!!! Не реализовано копирование разной хрени, кроме nMaptrix (pMatrix, wMatrix и пр.), так что в объекте копии нужно снова их создавать

    if(MatrixCopy.nMatrix) //#Копирование массива из исходной матрицы в текущую
    {
        nMatrix = new double[elements];
        std::copy(MatrixCopy.nMatrix, MatrixCopy.nMatrix + MatrixCopy.elements, nMatrix);
    }

}

MatrixClass::MatrixClass(MatrixClass const& Matrix1, MatrixClass const& Matrix2)
{
    if(Matrix1.cols != Matrix2.rows)
    {
        printf("Количество столбцов в первой матрице не равно количеству строк во второй. Перемножение матриц не возможно\n");
        _getch();_getch();
    }
    FlushVariables();
    rows     = Matrix1.rows;
    cols     = Matrix2.cols;
    elements = rows*cols;

    nMatrix = new double[rows * cols];
    for(int i = 0; i < rows;i++)
        for(int j = 0; j < cols;j++)
        {
            double scalar = 0;
            for(int k = 0; k < Matrix1.cols;k++)    //#Скалярное умножение элементов i-ой строки первой матрицы на элементы j-ого столбца второй матрицы
            {
                scalar += Matrix1.getElem(i , k) * Matrix2.getElem(k , j);
            }
            *(nMatrix + i*rows + j) = scalar;
        }
}

MatrixClass::~MatrixClass()
{
    //printf("MatrixClass::~MatrixClass()\n");
    delete[] nMatrix;
    if(pMatrix != nMatrix)delete[] pMatrix;
    if(wMatrix != nMatrix)delete[] wMatrix;
    if(bMatrix != nMatrix)delete[] bMatrix;
    if(rMatrix != nMatrix)delete[] rMatrix;
    if(cMatrix != nMatrix)delete[] cMatrix;
    if(kMatrix != nMatrix)delete[] kMatrix;
    if(uMatrix != nMatrix)delete[] uMatrix;
    if(iMatrix != nMatrix)delete[] iMatrix;
}

void MatrixClass::Transpose()
{
    double *tempmatrix = new double[cols*rows];
    //memcpy(tempmatrix, nMatrix, rows*cols*sizeof(double));
    for(int i = 0; i < rows*cols;i++)tempmatrix[i] = nMatrix[i];  //Копирование элементов во временную матрицу
    for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
    {
        *(nMatrix + j*cols + i) = *(tempmatrix + i*rows + j);  //Возможно что-то перепутал для неквадратной матрицы
    }
    delete tempmatrix;
}

void MatrixClass::PMatrix()
{
    pMatrix = new double[cols];
    for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
    {
        if(i == j)   //Если элемент диагональный
        {
            pMatrix[i] = *(nMatrix + i*rows + j);
        }
    }
    rows = 1;
    delete[] nMatrix;
    nMatrix = pMatrix;
}

void MatrixClass::WMatrix()
{
    wMatrix = new double[rows*cols];
    for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
    {
        if(i == j)   //Если элемент не диагональный
        {
            wMatrix[i*rows + j] = 0;
        }
        else
        {
            wMatrix[i*rows + j] = *(nMatrix + i*rows + j);
        }
    }
    delete[] nMatrix;
    nMatrix = wMatrix;
}

void MatrixClass::NeuroConvert()
{
    for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
    {
        if(i != j)   //Если элемент не диагональный
        {
            *(nMatrix + i*rows + j) = -*(nMatrix + i*rows + j);
        }
    }
}

void MatrixClass::RowMatrix(int row)
{
    rMatrix = new double[cols];
    for(int j = 0; j < cols; j++)
    {
        rMatrix[j] = *(nMatrix + row*cols + j);
    }
    rows = 1;
    delete[] nMatrix;
    nMatrix = rMatrix;
}

void MatrixClass::ColMatrix(int col)
{
    cMatrix = new double[rows];
    for(int i = 0; i < rows; i++)
    {

        cMatrix[i] = *(nMatrix + i*cols + col);
        printf("cMatrix[i] = %f\n", cMatrix[i]);
    }
    cols = 1;
    delete[] nMatrix;
    nMatrix = cMatrix;
    PrintMatrixData("ColMatrix\n");
}

void MatrixClass::CrossOutMatrix(int row, int col)
{
    //printf("CrossOutMatrix, row = %i, col = %i\n", row, col);
    //printf("CrossOutMatrix, rows = %i, cols = %i\n", rows, cols);
    //printf("nMatrix = %X\n", nMatrix);
    kMatrix = new double[(rows-1)*(cols-1)];
    int row_eraser = 0, col_eraser = 0; //#Коэффициенты, учитываеющие уменьшение матрицы на одну размерность
    for(int i = 0;i < rows;i++)
    {
        col_eraser = 0;
        if(i != row)    //#Если текущая строка не является вычеркиваемой
        {
            for(int j = 0;j < cols;j++) //#Если текущая колонка не является вычеркиваемой
            {
                if(j != col)   //Если элемент не диагональный
                {
                    *(kMatrix + (i - row_eraser)*(rows - 1) + (j - col_eraser)) = *(nMatrix + i*rows + j);
                }
                else    //#Если текущая колонка вычеркиваемая, то
                        //#1. она не добавляется к матрице,
                        //#2. со следующего шага номер колонки, формируемой матрицы на 1 меньше номера колонки матрицы источнкиа
                {
                    col_eraser = 1;
                }
            }
        }
        else
        {
            row_eraser = 1;
        }
    }
    //printf("CrossOutEnd\n");
    rows--;
    cols--;
    delete[] nMatrix;
    nMatrix = kMatrix;
}

void MatrixClass::UMatrix()
{
    printf("UMatrix::nMatrix = %X\n", nMatrix);
    uMatrix = new double[rows*cols];
    //MatrixClass* currentCrossOutMatrix; //#Текущая матрица получаемая вычеркиванием
    double det;
    int parity;  //#Четность
    for(int j = 0;j < cols;j++)for(int i = 0;i < rows;i++)
    {
        //currentCrossOutMatrix = new MatrixClass(this, MATRIX_TYPE_CROSSOUT, i, j);  //#Формирование матрицы вычеркиванием i-строки и j-столбца
        MatrixClass currentCrossOutMatrix(this, MATRIX_TYPE_CROSSOUT, i, j);  //#Формирование матрицы вычеркиванием i-строки и j-столбца
        det = currentCrossOutMatrix.getDetermenant();                              //#Получение определителя матрицы, полученной вычеркиванием
        //printf("det = %lf\n", det);
        parity = ((i + j) % 2 == 0)? 1 : -1;                                        //#Определение четности суммы i + j
        *(uMatrix + j*rows + i) = parity * det;                                     //#Формирование массива союзной матрицы (учтено транспонирование)
        //#delete currentCrossOutMatrix; //# !!! Поидее выполняется при завершении цикла автоматически.
                                         //#Оказывается не выполняется если в цикле матрица создается через new, однако если
                                         //#объект просто инициаллизируется в цикле до его деструктор вызывается в конце каждого цикла

    }
    delete[] nMatrix;     //#Освобождение памяти отведенной под исходные значения элементов
    nMatrix = uMatrix;
}

void MatrixClass::IMatrix()
{
    iMatrix = new double[rows*cols];
    double det;
    det = getDetermenant();  //#Получение определителя текущей матрицы
    UMatrix();               //#Преобразование текущем матрицы к союзной матрице
    if(det != 0.0)
    {
        for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
        {
            *(iMatrix + i*rows + j) = *(nMatrix + i*rows + j) / det;                                       //#Формирование массива обратной матрицы
        }
    }
    else
    {
        printf("Численное вычисление обратной матрицы не возможно, определитель равен 0\n");
        for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
        {
            *(iMatrix + i*rows + j) = 87;                                       //#Формирование массива обратной матрицы
        }
    }
    delete[] nMatrix;
    nMatrix = iMatrix;

}

void MatrixClass::ChangeCol(int col1, int col2)
{
    double temp;
    for(int i = 0;i < rows;i++)
    {
        temp = nMatrix[i*rows + col1];
        nMatrix[i*rows + col1] = nMatrix[i*rows + col2];
        nMatrix[i*rows + col2] = temp;
    }
}

void MatrixClass::ChangeRow(int row1, int row2)
{
    double temp;
    for(int i = 0;i < cols;i++)
    {
        temp = nMatrix[row1*rows + i];
        nMatrix[row1*rows + i] = nMatrix[row2*rows + i];
        nMatrix[row2*rows + i] = temp;
    }
}

void MatrixClass::MultipleCol(int col, double mul)
{
    for(int i = 0;i < rows;i++)
    {
        nMatrix[i*cols + col] *= mul;
    }
}

void MatrixClass::MultipleAll(double mul)
{
    for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
    {
            *(nMatrix + i*cols + j) *= mul;
    }
}

void MatrixClass::MultipleDiag(double mul)
{
    for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
    {
        if(i == j)   //Если элемент диагональный
        {
            nMatrix[i*rows + j] *= mul;
        }
    }
}

void MatrixClass::MultipleNotDiag(double mul)
{
    for(int i = 0;i < rows;i++)for(int j = 0;j < cols;j++)
    {
        if(i != j)   //Если элемент yt диагональный
        {
            nMatrix[i*cols + j] *= mul;
        }
    }
}


void MatrixClass::AddCol(int col, double add)
{
    for(int i = 0;i < rows;i++)
    {
        nMatrix[i*cols + col] += add;
    }
}

double MatrixClass::getElem(int row, int col) const
{
    if(row > rows || col > cols)
    {
        printf("Ощибка в параметрах функции getElem\n");
        return -12345678;
    }
    return *(nMatrix + cols*row + col);
}

int MatrixClass::getElemCount() const
{
    *(nMatrix + 5) = 10;
    return elements;
}

//double determ(int** Arr, int size)
double MatrixClass::getDetermenantRecursive(MatrixClass const *matr, int size)
{
    //printf("getDetermenantRecursive, size = %d, matr->nMatrix = %X\n", size, matr->nMatrix);
    double det = 0;
    if(size == 1)
    {
            det = matr->getElem(0,0);
    }
    else if(size == 2)
    {
            det = matr->getElem(0,0) * matr->getElem(1,1) - matr->getElem(0,1)*matr->getElem(1,0);
    }
    //else if(size == 3)  //Можно для ускорения добавить определитель 3-его порядка
    else
    {
            for(int j = 0; j < size;j++)    //#Разложение по строке первой (0-ой строке)
            {
                MatrixClass matr_recursive(matr, MATRIX_TYPE_CROSSOUT, 0, j);   //#Формирование матрицы вычеркиванием первой строки и j-ой колонки
                int parity = (j % 2 == 0)? 1 : -1;  //#Определение четности суммы i
                det += parity * MatrixClass::getDetermenantRecursive(&matr_recursive, size-1) * matr->getElem(0, j);
            }
    }
    //printf("det = %lf\n", det);
    return det;
}

//#Вывести содержимое матрицы на экран
void MatrixClass::PrintMatrixData(char* message)
{
    printf("\n");
    if(message) //#Вывод пригласительного сообщения
    {
        printf(message);
    }

    printf("rows = %d, cols = %d\n", rows, cols);
    for(int i = 0;i < rows;i++)
    {
        for(int j = 0;j < cols;j++)
        {

            //cout << "i = " << i << ", j = " << j << " ,*(nMatrix + i*rows + col) = " << *(nMatrix + i*rows + j) << endl;
            printf("i = %d, j = %d, elem = %f\n",i , j , getElem(i, j));

        }
    }

}



/*
int det(int aa[10][10],int n)
{
    int d=0;//а не глобально!!![/COLOR]
    int b[10][10];
    if(n==2)
    {
        return (aa[0][0]*aa[1][1]-aa[0][1]*aa[1][0]);
    }


   for(int i=0;i<n;i++)
   {
    for(int y=1; y<n; y++)
            for(int x=0; x<n; x++)
            {
                if(x==i) continue;
                if(x<i)
                    b[y-1][x] = aa[y][x];
                else
                    b[y-1][x-1] = aa[y][x];
            }
        d+=pow(-1,(double)(2+i))*aa[0]*det(b,n-1);
   }

   return d;
}*/

double MatrixClass::getDetermenant() const
{
    return getDetermenantRecursive(this, rows);
}
