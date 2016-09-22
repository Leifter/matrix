#include <QCoreApplication>
#include <QtXml>
#include <QFile>
#include "MatrixWork.h"
#include "neuroprocess.h"

QTextStream cin(stdin);
QTextStream cout(stdout);


void PrintReverseMatrix()
{
    int dimention = 4;
    int rows = dimention;
    int cols = dimention;

    double synapsMatrix[rows][cols] = {    //#matrix X
        {1.002, 1.001, 0.001, 1.003},
        {1.004, 0.003, 1.002, 1.003},
        {0.006, 0.007, 0.007, 0.002},
        {0, 0, 0, 0}
    };

    MatrixClass *matrix;
    matrix = new MatrixClass((double*)synapsMatrix, rows, cols);
    matrix->UMatrix();
    matrix->PrintMatrixData("uMatrix\n");

}
    slowEnum NeuroProcessSlow;

void ParseCmdLine(int argc, char *argv[])
{
    if(argc > 1)
    {
        NeuroProcessSlow = (slowEnum)atoi(argv[1]);
    }
    else
    {
        NeuroProcessSlow = SLOW_20MS;
    }
}

int main(int argc, char *argv[])
{
/*    double charge;
    double p;
    charge = atof(argv[1]);
    p = atof(argv[2]);
    int v_delta = charge / p;
    charge = charge - v_delta*p;
    printf("v_delta = charge / p = %d\n", v_delta);
    printf("charge = charge - v_delta*p = %lf\n", charge);
    return 0;
    */
    ParseCmdLine(argc, argv);

    MatrixClass *matrix, *matrix1, *matrix2, *matrix3;
    int dimention = 3;
    int rows = dimention;
    int cols = dimention;
    MatrixWork matrixWork;


#ifdef PRINT_REVERSE_MATRIX
    PrintReverseMatrix();
    return 0;
#endif

#ifdef ONE_NEURON_EXPEREMENTS
    int in;
    if(argc != 7)
    {
        printf("Vvedeno tolko %d parametrov, a neobhodimo 7\n", argc);
        return 1;
    }

    double pf = atof(argv[1]);
    double ws = atof(argv[2]);
    double b  = atof(argv[3]);
    double ch = atof(argv[4]);
    int b_times = atoi(argv[5]);                    //#Кол-во циклов внешнего воздействия, -1 бесконечно
    slowEnum slow_mode = (slowEnum)atoi(argv[6]);   //#параметр замедления. Замедление циклов,0-нет замедления,1-выводятся данные каждого цикла и задержка между циклами 20мс, 2 тоже, только задержка 200мс

    double *mat_mas_write, *mat_mas_read;

    setlocale(LC_ALL,"RUS");						//разрешение проблем с русским текстом в консоли. голова setlocale - locale.h

    double *pArray_t = new double[1];          //#Формирование матрицы внешних воздействий
    double *wArray_t = new double[1];          //#Формирование матрицы внешних воздействий
    double *bArray_t = new double[1];          //#Формирование матрицы внешних воздействий
    double *chArray_t = new double[1];         //#Формирование матрицы внешних воздействий

    pArray_t[0] = pf;
    wArray_t[0] = ws;
    bArray_t[0] = b;
    chArray_t[0] = ch;

    MatrixClass* matrix_neuro_read_p_t = new MatrixClass(pArray_t, 1, 1);     //#Формирование матрицы порогов
    MatrixClass* matrix_neuro_read_w_t = new MatrixClass(wArray_t, 1, 1);     //#Формирование матрицы весов
    MatrixClass bMatrix_t  = MatrixClass(bArray_t, 1, 1);
    MatrixClass chMatrix_t = MatrixClass(chArray_t, 1, 1);

    NeuroProcess(matrix_neuro_read_w_t, matrix_neuro_read_p_t, &bMatrix_t, &chMatrix_t, b_times, slow_mode); //Вызов нейропроцесса с такими данными
    NeuroProcess(matrix_neuro_read_w_t, matrix_neuro_read_p_t, &bMatrix_t, &chMatrix_t, 200, SLOW_NONE); //Вызов нейропроцесса с такими данными
    //NeuroProcess(matrix_neuro_read_w_t, matrix_neuro_read_p_t, &bMatrix_t, &chMatrix_t, NEURO_PROCESS_INFINATE, SLOW_NONE); //Вызов нейропроцесса с такими данными

    return 1;
#endif


/*    double synapsMatrix[rows][cols] = {    //#matrix X
        {10, 10, 4, 3},
        {1, 13, 5, 4},
        {1, 11, 20, 5},
        {1, 18, 4, 15}
    };
    */

    double synapsMatrix[rows][cols] = {    //#matrix X
        {10, 10, 4},
        {1, 13, 5},
        {1, 11, 20}
    };



/*    double synapsMatrix[rows][cols] = {    //#matrix X
        {10, 3},
        {3, 10}
    };
    */


    matrix = new MatrixClass((double*)synapsMatrix, rows, cols);


    //matrix = matrixWork.genMatrix(rows, cols);
    printf("Matrix generated\n");
    MatrixClass* matrix_neuro = new MatrixClass(matrix, MATRIX_TYPE_TONEURO); //#Получить матрицу, сконвертированную в обычное представление

//#define MX_WRITE   //Производить ли запись матрицы в xml файл. Закомментировать если не нужно

#ifdef MX_WRITE
    int wRes = matrixWork.writeMatrix(QString("matrixout.xmcd"), matrix_neuro);
    if(!wRes)cin >> in;   //Ошибка записи
    printf("Matrix written\n");
#endif
//NormalMatrix:
    MatrixClass* matrix_neuro_read = matrixWork.readMatrix(QString("matrixout.xmcd"), rows, cols);
    if(!matrix_neuro_read)
    {
        printf("matrix_neuro_read Error\n");
        return 0;
    }


    /*matrix_neuro_read->PrintMatrixData("nMatrix\n");
    printf("determ = %f\n", matrix_neuro_read->getDetermenant());
    matrix_neuro_read->UMatrix();
    matrix_neuro_read->PrintMatrixData("UMatrix\n");
    return 1;
    */

    matrix_neuro_read->NeuroConvert();  //#Преобразование матрицы к нейровиду. Не комментировать

 //#Различные эксперементальные преобразования матрицы
    //matrix_neuro_read->ChangeRow(0,4);
    //matrix_neuro_read->AddCol(2, 0.4);
    //matrix_neuro_read->MultipleDiag(-0.22);
    //matrix_neuro_read->MultipleNotDiag(1);
    //matrix_neuro_read->MultipleDiag(0.431666);  //min stabilniy
    //matrix_neuro_read->MultipleDiag(0.4315);  //min stabilniy
    //matrix_neuro_read->MultipleDiag(-0.2);
    //matrix_neuro_read->ChangeCol(0,2);
    //matrix_neuro_read->ChangeCol(1,2);
    //matrix_neuro_read->MultipleAll(0);
    //matrix_neuro_read->MultipleAll(10);
    //matrix_neuro_read->MultipleCol(0,100);
    /*matrix_neuro_read->ChangeCol(0,9);
    matrix_neuro_read->ChangeCol(1,8);
    matrix_neuro_read->ChangeCol(2,7);
    matrix_neuro_read->ChangeCol(3,6);*/
    //matrix_neuro_read->ChangeCol(4,4);
    //matrix_neuro_read->ChangeCol(1,4);
    //matrix_neuro_read->ChangeCol(1,4);
    //matrix_neuro_read->ChangeCol(1,4);
    //matrix_neuro_read->ChangeCol(1,4);

    matrix_neuro_read->PrintMatrixData("matrix_neuro_read->nMatrix\n");    //#Вывести на экран значение прочитанной матрицы

    /*matrix_neuro_read->Transpose();
    printf("nMatrix.Transpose()\n");
    for(int i = 0; i < rows; i++)for(int j = 0; j < cols; j++)
    {
        double elem = matrix_neuro_read->getElem(i , j);
        printf("elem = %f, i = %d, j = %d\n", elem, i, j);
    }*/

    MatrixClass* matrix_neuro_read_p = new MatrixClass(matrix_neuro_read, MATRIX_TYPE_PMATRIX);     //#Формирование матрицы порогов
    MatrixClass* matrix_neuro_read_w = new MatrixClass(matrix_neuro_read, MATRIX_TYPE_WMATRIX);     //#Формирование матрицы весов

//    matrix_neuro_read_w->PrintMatrixData("matrix_neuro_read_w\n"); //#Вывести на экран значение матрицы весов
//    matrix_neuro_read_p->PrintMatrixData("matrix_neuro_read_p\n"); //#Вывести на экран значение матрицы порогов

    double *bArray = new double[cols];          //#Формирование матрицы внешних воздействий
    for(int i = 0; i < cols; i++)bArray[i] = 5;
    MatrixClass bMatrix = MatrixClass(bArray, 1, cols);

    double *bArray0 = new double[cols];         //#Формирование матрицы внешних 0-евых воздействий
    for(int i = 0; i < cols; i++)bArray0[i] = 0;
    MatrixClass bMatrix0 = MatrixClass(bArray0, 1, cols);

    double *bArrayStab = new double[cols];         //#Формирование матрицы внешних воздействий соответствующих сумме элементов строки
    for(int i = 0; i < cols; i++)
    {
        double sum = 0;
        for(int j = 0;j < cols;j++) //#Получение суммы элементов исходной матрицы
        {
            sum += matrix_neuro->getElem(i, j);
        }
        bArrayStab[i] = sum;
    }
    MatrixClass bMatrixStab = MatrixClass(bArrayStab, 1, cols);


    double *chArray = new double[cols];         //#Формирование матрицы начальных зарядов нейронов
    for(int i = 0; i < cols; i++)chArray[i] = 0;
    MatrixClass chMatrix = MatrixClass(chArray, 1, cols);

    double *chArrayP = new double[cols];         //#Все начальные заряды соответствуют порогу, нейроны изначально активируются
    for(int i = 0; i < cols; i++)chArrayP[i] = matrix_neuro_read_p->nMatrix[i];
    MatrixClass chMatrixP = MatrixClass(chArrayP, 1, cols);

    /*
    printf("matrix_neuro_read_w = %0x\n", matrix_neuro_read_w->wMatrix);
    printf("matrix_neuro_read_p = %0x\n", matrix_neuro_read_p->wMatrix);
    printf("bMatrix = %0x\n", &bMatrix.wMatrix);
    printf("chMatrix = %0x\n", &chMatrix.wMatrix);
    */
//    cin >> in;

    //bMatrixStab.PrintMatrixData("bMatrixStab\n");
    //return 1;
    //getchar();
    //NeuroProcess(matrix_neuro_read_w, matrix_neuro_read_p, &bMatrixStab, &chMatrix, NEURO_PROCESS_INFINATE, NeuroProcessSlow); //Вызов нейропроцесса с такими данными
    NeuroProcess(matrix_neuro_read_w, matrix_neuro_read_p, &bMatrix, &chMatrix, NEURO_PROCESS_INFINATE, NeuroProcessSlow); //Вызов нейропроцесса с такими данными
    //NeuroProcess(matrix_neuro_read_w, matrix_neuro_read_p, &bMatrix0, &chMatrixP, NEURO_PROCESS_INFINATE, NeuroProcessSlow); //Вызов нейропроцесса с такими данными

    //printf("return 1\n");

    /*    QCoreApplication a(argc, argv);

        XMLWork xmlwork;
        return a.exec();*/

        //QTextStream cout(stdout);

       // makeDocument();
        //addDocument();
        //makeMatrix();
        //int in;
        //cin >> in;
    return 0;
}

