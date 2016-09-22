//Класс генерация, чтения и записи матрицы

#include "MatrixWork.h"

MatrixWork::MatrixWork()
{

}

MatrixWork::~MatrixWork()
{

}

MatrixClass *MatrixWork::genMatrix(int rows, int cols)
{
    double *matrix_mas = new double[rows*cols];
    srand(time(0));
    for(int j = 0; j < cols; j++)for(int i = 0; i < rows; i++)
    {
        if(i == j && false)
        {
            matrix_mas[i*rows + j] = (double)rand()/30000;
            //printf();
        }
        else
        {
            matrix_mas[i*rows + j] = (double)rand()/30000 - 0.5;
        }

    }

    MatrixClass *matrix = new MatrixClass(matrix_mas, rows, cols);
    delete []matrix_mas;
    return matrix;
}

MatrixClass *MatrixWork::readMatrix(QString filename, int &rows, int &cols)
{
    QDomDocument domDoc;              //#Создаем документ
    QFile        file(filename);      //#Открываем файл с документом
    double *matrix_buf;

    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {                                                 //#Устанавливаем содержимое документа в соответствии с содержимым файла
            QDomElement domElement = domDoc.documentElement();                         //#Получаем указатель на корневой элемент документа
            matrix_buf = getMatrixRowsCols(domDoc, rows, cols);     //Получение матрицы и ее рвзмеров
        }
        file.close();
    }
    else{
        printf("Error open matrix file\n");
        return 0;
    }
    MatrixClass *matrix_read = new MatrixClass(matrix_buf, rows, cols); //Создание объекта матрицы
    matrix_read->Transpose();                                           //Транспонирование матрицы, т.к. из хмл она считывается по колонкам
    return matrix_read;
    //return TransposeMatrix(matrix_buf, rows, cols);
}

double *MatrixWork::getMatrixRowsCols(const QDomDocument &doc, int &rows, int &cols)
{
    QDomNodeList *domNodeList = new QDomNodeList();
    QDomElement *domElement = new QDomElement();
    *domNodeList = doc.elementsByTagName(QString("ml:matrix"));  //№Получение тага с матрицей
    if(domNodeList->isEmpty())
    {
        printf("Element not found (matrix rows cols))\n");
        return 0;
    }
    else{
        printf("Element found (matrix rows cols)\n");
        *domElement = domNodeList->at(0).toElement();
    }
    rows = domElement->attribute(QString("rows")).toInt();   //#Получение количества колонок
    cols = domElement->attribute(QString("cols")).toInt();   //#Получение количества колонок

    *domNodeList = doc.elementsByTagName(QString("ml:real")); //№Получение списка нодов с тагами элементов матрицы
    if(domNodeList->isEmpty())
    {
        printf("Elements not found (matrix elements)\n");
        return 0;
    }
    else{
        printf("Elements found (matrix elements)\n");
    }

    //QDomText *domText = new QDomText();
    double *nMatrix = new double[domNodeList->count()];
    for(int i = 0; i < domNodeList->count(); i++)
    {
        *domElement = domNodeList->at(i).toElement();
        nMatrix[i] = domElement->text().toDouble();
    }
    return nMatrix;
}

int MatrixWork::writeMatrix(QString filename, MatrixClass* matrix)
{
    QDomDocument domDoc;                  //#Создаем документ
    QFile        file("matrixpat.xml");   //#Открываем файл с документом

    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {                                                 //#Устанавливаем содержимое документа в соответствии с содержимым файла
            QDomElement domElement = domDoc.documentElement();                         //#Получаем указатель на корневой элемент документа
            QDomElement ml_define = findFirstElementByTag(domDoc, "ml:define");    //#Функция поиска тела матрицы
            QDomElement matrix_el = matrixElement(domDoc, matrix);        //#Формирование элемента матрицы
            ml_define.appendChild(matrix_el);                                             //#Добавление матрицы в тело
        }
        file.close();
    }
    else{
        printf("Error open matrix pattern\n");
        return 0;
    }

    QFile file2(filename);  //№Создание выходного файла с матрицей
    if(file2.open(QIODevice::WriteOnly)) {
        QTextStream(&file2) << domDoc.toString(); //№Запись файла
        file2.close();
        printf("Matrix file written\n");
    }
    else{
        printf("Error write matrix file\n");
        return 0;
    }

    return 1;
}

QDomElement MatrixWork::findFirstElementByTag(const QDomDocument &doc, const QString &tagName)
{
    QDomNodeList *domNodeList = new QDomNodeList();
    QDomElement *domElement = new QDomElement();
    *domNodeList = doc.elementsByTagName(tagName);
    if(domNodeList->isEmpty())
    {
        printf("Element not found metka 1\n");
        //printf("Element not found %s\n", tagName.);
        return *domElement;
    }
    else{
        printf("Element found\n");
        *domElement = domNodeList->at(0).toElement();
        return *domElement;
    }
    return *domElement;

}

//#Функция формирования элемента матрицы
QDomElement MatrixWork::matrixElement(QDomDocument& domDoc, MatrixClass* matrix)
{
    int rows,cols;
    rows = matrix->rows;
    cols = matrix->cols;
    QDomElement domElement = makeMatrixElement(domDoc, QString("ml:matrix"), rows, cols, NULL);

    for(int j=0; j < cols; j++)for(int i=0; i < rows; i++)
    {
        domElement.appendChild(makeMatrixElement(domDoc, "ml:real", NULL, NULL, *(matrix->nMatrix + i*rows + j)));
    }

    return domElement;
}

QDomElement MatrixWork::makeMatrixElement(QDomDocument& domDoc, const QString& strName, int rows, int cols, double real)
{
    QDomElement domElement = domDoc.createElement(strName);     //#Создание элемента с именем strName

    if (rows && cols) {                                      //#Если указано значение, то создается элемент контакта и к нему добавляются аттрибуты
        QDomAttr domAttr;
        domAttr = domDoc.createAttribute("cols");
        domAttr.setValue(QString().setNum(cols));                              //#Установка значение атрибута контакта
        domElement.setAttributeNode(domAttr);
        domAttr = domDoc.createAttribute("rows");    //#Формирование аттрибута "number" контакта,
        domAttr.setValue(QString().setNum(rows));                              //#Установка значение атрибута контакта
        domElement.setAttributeNode(domAttr);
    }

    if (!rows && !cols) {                                   //#Если указан текста, то создается текстовый узел
        QDomText domText = domDoc.createTextNode(QString().setNum(real, 'f', 10));      //#Установка текста узла
        domElement.appendChild(domText);                        //#Добавление текста как дочернего узла к созданному
    }
    return domElement;                                          //#Возврат созданного элемента (или контакта или одного из содержимого контакта)
}
