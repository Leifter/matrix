#ifndef MATRIXWORK_H
#define MATRIXWORK_H

#include <QtXml>
#include <QFile>
#include <QDomNodeList>
#include <QVector>

#include <math.h>
#include <ctime>

#include "MatrixClass.h"

class MatrixWork
{
public:
//    double *matrix;
    MatrixWork();
    ~MatrixWork();
    MatrixClass*  genMatrix(int rows, int cols);                                          //#генерация матрицы
    MatrixClass*  readMatrix(QString filename, int &rows, int &cols);                     //#чтение данных из автокадовского файла матрицы
    int           writeMatrix(QString filename, MatrixClass *matrix);      //#запись данных в файл матрицы

    QDomElement findFirstElementByTag(const QDomDocument &doc, const QString &tagName); //#Функция поиска первого элемента в xml по тагу
    QDomElement matrixElement(QDomDocument& domDoc, MatrixClass *matrix);               //#Формирование всех элементов матрицы
    QDomElement makeMatrixElement(QDomDocument& domDoc, const QString& strName, int rows, int cols, double real); //#Формирование элемента матрицы
    double      *getMatrixRowsCols(const QDomDocument &doc, int &rows, int &cols); //Получение из XML матрицы. !!!В xml должна быть только одна матрица!!!

};

#endif // MATRIXWORK_H
