#ifndef NEUROPROCESS_H
#define NEUROPROCESS_H

#include "MatrixClass.h"

#define NEURO_PROCESS_INFINATE -1

enum slowEnum{          //#Варианты замедления
    SLOW_NONE = 0,
    SLOW_20MS = 1,
    SLOW_1000MS = 2
};

void gotoxy(int,int);   //функция установки позиции курсора в консоли
int NeuroProcess(MatrixClass *wMatrix, MatrixClass *pMatrix, MatrixClass *bMatrix, MatrixClass *chMatrix, int bCount, slowEnum slow);

#endif // NEUROPROCESS_H
