#ifndef MATRIXCLASS_H
#define MATRIXCLASS_H

#include "stdlib.h"
#include "stdio.h"
#include "conio.h"

enum MatrixType{
    MATRIX_TYPE_COPY = 0,    //Создание копии матрицы
    MATRIX_TYPE_TRANSPOSE,   //Создание транспонированной матрицы
    MATRIX_TYPE_PMATRIX,     //Создание РМатрицы
    MATRIX_TYPE_WMATRIX,     //Создание WМатрицы
    MATRIX_TYPE_TONEURO,     //Создание матрицы в нейро-представлении
    MATRIX_TYPE_FROMNEURO,   //Создание матрицы в нейро представлении матрицы в обычном виде
    MATRIX_TYPE_ROW,         //Создание матрицы строки
    MATRIX_TYPE_COLUMN,      //Создание матрицы колонки
    MATRIX_TYPE_CROSSOUT,    //Создание матрицы путем вычеркивания строки и столбца
    MATRIX_TYPE_UNION,       //Создание союзной матрицы
    MATRIX_TYPE_INVERT       //Создание обратной матрицы
};

#define ANYVALUE NULL     //Означает, что для данной функции или метода переменная не имеет значения

class MatrixClass
{
public:
    void          FlushVariables();                                                       //#Обнеление констант матрицы
    MatrixClass();
    MatrixClass(double *matrix_mas, int rows, int cols);                                  //#Формирование матрицы по массиву, количеству строк и столбцов
    MatrixClass(MatrixClass const*matrix, MatrixType type, int row = 0, int col = 0);     //#Формирование из данной матрицы матрицы другого типа
    MatrixClass(MatrixClass const& MaxrixCopy);                                           //#Конструктор копироваия
    MatrixClass(MatrixClass const& Matrix1, MatrixClass const& Matrix2);                  //#Перемножение матриц
    ~MatrixClass();
    void          Transpose();                                                            //#транспонирование матрицы
    void          PMatrix();                                                              //#Превращение матрицы в PМатрицу
    void          WMatrix();                                                              //#Превращение матрицы в WMaтрицу
    void          NeuroConvert();                                                         //#Преобразование матрицы в нейро-матрицу с инверсией всех кроме диагональных элементов и обратно
    void          RowMatrix(int rowcol);                                                  //#Преобразование матрицы к матрице строке
    void          ColMatrix(int rowcol);                                                  //#Преобразование матрицы к матрице столбцу
    void          CrossOutMatrix(int row, int col);                                       //#Получение матрицы вычеркиванием колонки и столбца
    void          UMatrix();                                                              //#Получение союзной матрицы
    void          IMatrix();                                                              //#Получение обратной матрицы

    void          ChangeCol(int col1, int col2);                                          //#Поменять местами колонки
    void          ChangeRow(int row1, int row2);                                          //#Поменять местами строки
    void          MultipleCol(int col, double mul);                                       //#Умножение всех элементов данной колонки на mul
    void          MultipleAll(double mul);                                                //#Умножение всех элементов матрицы на mul
    void          MultipleDiag(double mul);                                               //#Умножение диагонали матрицы
    void          MultipleNotDiag(double mul);                                            //#Умножение всех элементов кроме диагонали
    void          AddCol(int col, double add);                                            //#Прибавление значения к колонке

    double        getElem(int row, int col) const;                                        //#Получение элемента матрицы
    int           getElemCount() const;                                                   //#Получение количества элементов матрицы

    static double getDetermenantRecursive(MatrixClass const *, int size);                 //#вычисление определителя произвольной матрицы рекурсивно
    double        getDetermenant() const;                                                 //#вычисление определителя данной матрицы

    void          PrintMatrixData(char* message);                                         //#Вывести содержимое матрицы на экран

    ////double        getUnionMatrix();                                                       //#получение союзной матрицы

    int rows;
    int cols;
    int elements;

    double *nMatrix;    //#Указатель на массив обычной матрицу
    double *pMatrix;    //#Указатель на массив матрицы порогов
    double *wMatrix;    //#Указатель на массив матрицы весов
    double *bMatrix;    //#Указатель на массив матрицы внешних воздействий
    double *rMatrix;    //#Указатель на массив матрицы строки
    double *cMatrix;    //#Указатель на массив матрицы столбца
    double *kMatrix;    //#Указатель на массив матрицы, полученной вычеркиванием строки и столбца
    double *uMatrix;    //#Указатель на массив союзной матрицы
    double *iMatrix;    //#Указатель на массив обратной матрицы

};

#endif // MATRIXCLASS_H
