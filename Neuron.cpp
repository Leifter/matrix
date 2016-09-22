#include "StdAfx.h"
#define NEURO_CPP //Метка, чтобы не дефайнились лишние данные из synaps.h, а то ошибка ддвойной дефинации при линковке
#include "Neuron.h"

Neuron::Neuron(int num, int nn, MatrixClass* wMatrix, MatrixClass* pMatrix, MatrixClass* chMatrix, Neuron **neighbors)
{
    printf("Metka2");
    Neuron::p = pMatrix->nMatrix[num];   //установка порога нейрона
    //Neuron::isP = pPropierty[num];       //установка признака p-нейрона //Доработать
	Neuron::num = num;                   //установка номера нейрона
	Neuron::neurons_number = nn;         //установка общего количества нейронов
	Neuron::neighbors = neighbors;       //указатель на массив с соседями
    charge = chMatrix->nMatrix[num];     //присвоение начального заряда нейрона
    printf("Metka3");
    //MatrixClass *testm = new MatrixClass(wMatrix, MATRIX_TYPE_COLUMN, num);
    //wMatrix->PrintMatrixData("wMatrix\n");

    neuron_wMatrix = new MatrixClass(wMatrix, MATRIX_TYPE_COLUMN, ANYVALUE, num); //Получение матрицы колонки с весами воздействия данного нейрона

    /*
    double p_real = 1000;
    double p_w = p_real - Neuron::p;
    neuron_wMatrix->nMatrix[num] = p_w; //#Приблуда для взаимодействия с порогом
    Neuron::p = p_real;
    */

    vn = 0;                              //количество срабатываний нейрона
    vn_float = 0;                        //нецелочисленное количество срабатываний
    vn_abs = 0;							 //абсолютное количество срабатываний нейрона без учета инверсии
    charge_evalute = 0;					 //обнуление заряда следующего цикла
    charge_start += charge;              //увеличение счетчика стартового заряда
	fn = 0;                              //обнуление частоты нейрона
	numOfFerments = 0;                   //количество ферментаций нейрона
	vn_f = 0;
	vn_f_avg = 0;
	f_avg = 0;

    isP = 0;  //#Обнуление свойства нейрона

    //printf("p = %f\n", p);
    //_getch();

    //neuron_wMatrix->PrintMatrixData("neuron_wMatrix->nMatrix[]\n");
    //_getch();
    /*
/*    for(int i = 0; i < nn; i++)for(int j = 0; j < cols; j++)
    {
        double elem = wArray[i];
        printf("num = %d, elem = %f, i = %d, j = %d\n",num ,elem, i, j);
    }*/

	neuron_ch_in = 0;                    //сколько в нейрон поступило в данном такте
	neuron_ch_out = 0;                   //сколько нейрон отдал в данном такте
	neuron_ch_eat = 0;                   //сколько нейрон потратил в данном такте
	//printf("Pereddanniy arhument neighbors %0X\n", Neuron::neighbors);
}

Neuron::~Neuron(void)
{
    //printf("Destructor %d\n", num);
    delete neuron_wMatrix;
}

//Функция формирования заряда нейрона для следующего цикла
int Neuron::Ferment(double w){
    //if(isP == 1)if(charge < 0.0)charge=0; //если нейрон является p-нейроном, то его заряд не может быть меньше 0
	if(w != 0.0){
		numOfFerments++;                    //увеличение количества ферментаций нейрона
		charge_evalute += w;	     		//Увеличение заряда, формируемого для следующего цикла

	}
	return 0;
}

//Функция передачи заряда, накопленного в данном цикле следующему циклу
int Neuron::Evalute(){
    charge += charge_evalute;     //приращение заряда, оставшегося от данного цикла зарядом, накопленным в данном цикле
    charge_evalute = 0;		      //обнуление заряда, накопленный в данном цикле

    return 0;
}

//Функция активации нейрона по достижению порога
int Neuron::Activate()
{

	//Спорное решение так как в более сложных случаях может оказаться важным сохранять заряд, т.к. в последствии он может стать положительным, тогда просто return 0 при активации
	if(isP == 1)if(charge < 0.0){ //Свойство 1 означает p-нейрон, т.е. его заряд не может быть отрицателен, при этом ограничения на положительный заряд нет
		charge=0;  
		return 0;
	}
	if(isP == 2)if(charge!= 0.0){  //Свойство 2 означает, что нейрон срабатывает 1 раз при любом воздействии
		charge = p;
	}
    //int v_delta = 0;
    double v_delta_float = 0;
    //if(fabs(charge) >= p || charge == 0)
    //if(fabs(charge) >= p)
    /*charge = -1*charge;
    printf("num = %d, charge = %f\n", num, charge);
    printf("num = %d, fabs(charge) = %f\n", num, fabs(charge));
    _getch();*/
    //if(fabs(charge) >= fabs(p))
    //if(fabs(charge) >= p)
    if(fabs(charge) >= fabs(p))
	{
        //int v_delta = (int)(p != 0) ? (charge / p) : 1;      //изменение количества срабатываний (деление без остатка)
        //v_delta_float = (p != 0) ? (charge / p) : 1;
        //v_delta_float = fabs(v_delta_float);
//        int v_delta = (int)(charge != 0) ? (p / charge) : 1;      //изменение количества срабатываний (деление без остатка)
        //int v_delta = (charge >= p) ? 1 : -1;  //изменение количества срабатываний (деление без остатка)
        int v_delta = charge / p;
		vn += v_delta;					   //увеличение количества срабатывания нейронов
        /*printf("num = %d\n",num);
        printf("vn = %d\n",vn);
        printf("v_delta = %d\n",v_delta);
        printf("charge = %f\n",charge);
        printf("p = %f\n",p);*/

        //vn_float += v_delta_float;
        //charge -= v_delta*p;               //заряд равен остатку от деления
        //charge = v_delta*p - charge;     //уменьшение заряда в связи со срабатыванием
        //charge = 0.3*(charge - v_delta*p);     //заряд равен остатку от деления
        /*if(charge > v_delta*p)
        {
            charge = charge - v_delta*p;     //заряд равен остатку от деления
        }
        else
        {
            charge = -(charge - v_delta*p);     //заряд равен остатку от деления
        }*/
        charge = charge - v_delta*p;     //заряд равен остатку от деления
        //charge = 0;

        //fn = (fn + v_delta_float) / 2;           //Вычисление среднего количества срабатываний нейрона
		
        for(int i = 0;i < neurons_number;i++)
        {                                  //цикл по всем связанным нейронам
            double w = neuron_wMatrix->nMatrix[i];      //определение веса воздействия на соедененнй нейрон
            if(w != 0.0)
            {
                w = w*v_delta;              //в данном случае при отрицательной ферментации имеется инверсия w. Ее наличие может являться свойством нейрона а может им и не являться.
                //w = w*v_delta_float;          //в данном случае при отрицательной ферментации имеется инверсия w. Ее наличие может являться свойством нейрона а может им и не являться.
				neighbors[i]->Ferment(w);   //ферментировать i-ный связанный нейрон
			}
		}
	}
    //fn = (fn + (double)v_delta) / 2;           //Вычисление среднего количества срабатываний нейрона
    //fn = (fn + v_delta_float) / 2;           //Вычисление среднего количества срабатываний нейрона
	return 0;
}

long   Neuron::v = 0;			     //Количество циклов системы
double Neuron::f = 0;			     //Зарезервировано для тактовой частоты. Пока вопрос с ее существованием не ясен
double Neuron::charge_start = 0;     //начальный заряд системы
double Neuron::charge_in = 0;        //дабавление заряда в систему
double Neuron::charge_out = 0;       //выход заряда из системы
//добавить eat
double Neuron::charge_extern_in = 0; //внешнее добавление заряда в систему

double Neuron::determenant = 0;     //внешнее добавление заряда в систему
