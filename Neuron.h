#pragma once
//#include "synaps.h"
#include "MatrixClass.h"

class Neuron
{
private:

	static double f;                //зарезервированно для тактовой частоты
public:
		
    Neuron(int num, int nn, MatrixClass* wMatrix, MatrixClass* pMatrix, MatrixClass* chMatrix, Neuron **neighbors);
	~Neuron(void);
	int Ferment(double w);
	int Activate();
	int Evalute();

	Neuron **neighbors;

	long num;                   //номер нейрона
	long vn;                    //количество срабатываний нейрона
    double vn_float;            //нецелочисленное количество срабатываний
	long vn_abs;				//абсолютное количество срабатываний нейрона
	double fn;                  //частота нейрона
	double charge;              //заряд нейрона

	double charge_evalute;      //заряд, который будет использоваться в следующем цикле активации

    double vn_f;                 //значение частоты данного нейрона
    double vn_f_avg;             //среднее значение частоты
    long   f_avg;                //количество увеличений среднего значения

    double p;                    //порог срабатывания нейрона
//	double ws[NEURONS_NUMBER];   //массив с весами данного нейрона
    MatrixClass *neuron_wMatrix; //объект с матрицей весов воздействия данного нейрона

    int    isP;                  //является ли нейрон p-нейроном
    long   numOfFerments;        //количество внешних воздействий на нейрон. Пока не используется

	double neuron_ch_in;        //сколько в нейрон поступило в этом такте
	double neuron_ch_out;       //сколько нейрон сгенерировал в этом такте
	double neuron_ch_eat;       //сколько нейрон потратил в этом такте

	static long v;              //количество тактовых срабатываний

	//Оказалось, что описанные ниже переменные, по большей (не проверено с учетом charge_extern_in) части дублируют InstaChargе, расчитываемый циклом как сумма оставшихся зарядов, поэтому их применение под вопросом
	static double charge_start;     //начальный заряд системы
	static double charge_in;        //дабавление заряда в систему
	static double charge_out;       //выход заряда из системы
	static double charge_extern_in; //внешнее добавление заряда в систему

	static double determenant;      //счетчик вычесленных детерменантов

    int    neurons_number;      //общее количество нейронов 
};
