// NeuroProcess++.cpp : Calculate neuron network process.
//
#include "stdafx.h"
#include "neuron.h"
#include "MatrixClass.h"
#include "neuroprocess.h"

//#define NEURONS_NUMBER 4
//extern double bs[NEURONS_NUMBER];           //bs - массив внешних воздействий нейронов. инициаллизируется в neuron.cpp
//extern double chargeArray[NEURONS_NUMBER];  //массив первоначальных зарядов нейронов
/*!
  \brief Процесс работы нейронной сети

  \param wMatrix Матрица весов
  \param pMatrix Матрица порогов
  \param bMatrix Матрица внешних воздействий
  \param chMatrix Матрица начальных зарядов
  \param bCount Кол-во циклов внешнего воздействия, -1 бесконечно
  \param slow - параметр замедления. Замедление циклов,0-нет замедления,1-выводятся данные каждого цикла и задержка между циклами 20мс, 2 тоже, только задержка 200мс
*/
int NeuroProcess(MatrixClass *wMatrix, MatrixClass *pMatrix, MatrixClass *bMatrix, MatrixClass *chMatrix, int bCount, slowEnum slow)
{
    int NEURONS_NUMBER = wMatrix->cols;
    Neuron *NS[NEURONS_NUMBER];                     //массив для указателей на объекты нейронов. Нужно переделать в список
    double *bs = bMatrix->nMatrix;                  //bs есть массив внешних воздействий на нейроны
    //int NEURONS_NUMBER = pMatrix->cols();
	/*printf("%0X\n", NS);
	printf("%0X\n", *NS);
	printf("%0X\n", &NS);
	printf("%d\n", sizeof(NS));*/
    printf("Metka1");

	//Цикл создания нейронов сети
	for(int i = 0;i < NEURONS_NUMBER;i++){				 
        NS[i] = new Neuron(i, NEURONS_NUMBER, wMatrix, pMatrix, chMatrix, (Neuron**)NS);
	}
	printf("Kolichestvo neyronov v tekushey neyronnoy seti %d\n", NEURONS_NUMBER);
    //return 1;
//    return 1;
	double b;					//внешнее воздействие на нейрон
	int y = 20;
	for(;;){
		if(slow == 1)Sleep(20); 
        if(slow == 2)Sleep(200);
		//Внешние воздействия
        //if(Neuron::v < 200002)

        if(bCount > 0 || bCount == -1)
        {
            for(int i = 0;i < NEURONS_NUMBER;i++){	//цикл ферментации
                b = bs[i];           //получение внешнего воздействия
                if(b != 0.0){
                    NS[i]->Ferment(b);                //Ферментировать нейрон
                    //Neuron::charge_extern_in += b;    //Увеличение счетчика входящего заряда
                }
            }
            if(bCount != -1)bCount--;   //#Уменьшение счетчика циклов внешнего воздействия, если количество воздействий ограничего
        }

		//Активация нейронв
        for(int i = 0;i < NEURONS_NUMBER;i++){	//цикл активации
            NS[i]->Activate();   //провести активировацию нейрона
        }

		//Эволюция нейрона
		for(int i = 0;i < NEURONS_NUMBER;i++){	//цикл эволюции
			NS[i]->Evalute();   //провести эволюцию нейрона
		}

		//Попытка определения периода, по зарядам. Почему-то не работает. Нужно разобраться	
		/*for(int i = 0;i < NEURONS_NUMBER;i++){	//цикл проверки цикличности значений зарядов нейронов
			if(NS[i]->charge != 1.0){
				special2 = false;
				break;
			}
		}*/

        Neuron::v++;     //увеличение счетчика циклов (оно же максимальное число внешних воздействий)
    	//Вывод статистики
        if(Neuron::v % 10000 == 0 || slow){                                                        //Производить вывод сообщений только для каждого n-ого цикла
            gotoxy(0,1);
            for(int i = 0;i < NEURONS_NUMBER;i++){                                                  //цикл вывода частот на экран
                printf("v%d       =%f vn = %d         \n", i + 1,(double)NS[i]->vn/((Neuron::v == 0)? 1 : Neuron::v), NS[i]->vn );  //#вывод частоты срабатывания нейрона
                //printf("v_float%d =%f\n", i + 1,(double)NS[i]->vn_float/((Neuron::v == 0)? 1 : Neuron::v) );  //#вывод частоты срабатывания нейрона
                //printf("fn%d      =%f\n", i + 1,NS[i]->fn );  //#вывод частоты срабатывания нейрона
			}
            //printf("fn1/fn2 = %f\n", NS[0]->vn / (NS[1]->vn + 0.1));
            //printf("fn1/fn3 = %f\n", NS[0]->vn / (NS[2]->vn + 0.1));
            //printf("fn2/fn3 = %f\n", NS[1]->vn / (NS[2]->vn + 0.1));
            printf("Neuron::v = %d                 \n", Neuron::v);
		

            if(false){
				_getch();_getch();
			}
		}

	}
	
//	neuron->Ferment(20);
//	double v = neuron->GetV();
//	printf("Hello world %d\n", v);
	_getch();_getch();
	return 0;
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
