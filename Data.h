
#ifndef DATA_H
#define DATA_H

#include <QStringList>

typedef struct  {
    QVector<double>  Time     ;
    QVector<double>  AccX    ;
    QVector<double>  AccY    ;
    QVector<double>  AccZ    ;
    QVector<double>  GyroX   ;
    QVector<double>  GyroY   ;
    QVector<double>  GyroZ   ;
    QVector<double>  Pressure ;
    QVector<double>  Temperature     ;
    QVector<double>  RTDTemp  ;
    QVector<double>  MagX;
    QVector<double>  MagY;
    QVector<double>  MagZ;
    QVector<double>  OrientX;
    QVector<double>  OrientY;
    QVector<double>  OrientZ;
    QVector<double>  linear_acc_x;
    QVector<double>  linear_acc_y;
    QVector<double>  linear_acc_z;
    QVector<double>  Current;
    QVector<double>  BusVoltage;
    QVector<double>  ShuntVoltage;
    QVector<double>  AccCalib;
    QVector<double>  GyroCalib;
    QVector<double>  MagCalib;
    QVector<double>  SysCalib;
    QVector<double>  Counter;

} Sensor;
typedef struct  {
    uint32_t Time ;
    int32_t  AccX ;
    int32_t  AccY ;
    int32_t  AccZ ;
    int32_t  GyroX;
    int32_t  GyroY;
    int32_t  GyroZ;
    uint32_t  Pressure ;
    int32_t  Temperature     ;
    int32_t  RTDTemp  ;
    int32_t  MagX;
    int32_t  MagY;
    int32_t  MagZ;
    int32_t  OrientX;
    int32_t  OrientY;
    int32_t  OrientZ;
    int32_t  Current;
    int32_t  Busvoltage;
    int32_t  Shuntvoltage;
    uint8_t  AccCalib;
    uint8_t  GyroCalib;
    uint8_t  MagCalib;
    uint8_t  SysCalib;
    uint32_t Counter;

} Temp;
typedef struct{
    QVector<double>YAxisData1;
    QVector<double>YAxisData2;
    QVector<double>YAxisData3;

}Custom;

int windowsize   ;// =10000;      //Window size of the graph displyed

/*
   Variable that are used to keep track of no.of data points added.
   Once 100 data points are added to the plotdata their respective vector
   is cleared.
*/
int Time_counter           ;
int AccDataCounter         ;
int GyroDataCounter        ;
int PressureDataCounter    ;
int TemperatureDataCounter ;
int CustomDataCounter ;
int CalibDataCounter;
int VoltageDataCounter;
int MagDataCounter;
int RTDDataCounter;
int OrientDataCounter;
int CurrentDataCounter;

/*
  Variable that is used to count if data points equal to the size of the window
  has been added or not
*/

int  GyroPopulation         ;
int  AccPopulation          ;
int  PressurePopulation      ;
int  TemperaturePopulation  ;
int  CustomPopulation  ;
int  VoltagePopulation  ;
int  MagPopulation  ;
int  RTDPopulation  ;
int  OrientPopulation  ;
int  CurrentPopulation  ;

/*
 Circular  buffer used to store the value of time.This buffer is used to
 remove old data points when a new data point  is added.
*/
QVector<double> AccPreviousTime;
QVector<double> GyroPreviousTime;
QVector<double> PressurePreviousTime;
QVector<double> TemperaturePreviousTime;
QVector<double> CustomPreviousTime;
QVector<double> VoltagePreviousTime;
QVector<double> MagPreviousTime;
QVector<double> RTDPreviousTime;
QVector<double> OrientPreviousTime;
QVector<double> CurrentPreviousTime; //  Current as I not currently


int GyroPreviousTimeCounter        ;
int AccPreviousTimeCounter         ;
int PressurePreviousTimeCounter    ;
int TemperaturePreviousTimeCounter ;
int CustomPreviousTimeCounter ;
int VoltagePreviousTimeCounter ;
int MagPreviousTimeCounter ;
int RTDPreviousTimeCounter ;
int OrientPreviousTimeCounter ;
int CurrentPreviousTimeCounter ;


double TimerOffset;   // stores the timer offset
bool TimerOffsetCalculated;


bool StartedPlotting = false; // set to true once the user clicks start in the gui. True indicates that plotting has started

/*
 is set to true it indicates that the respective plot has been populated with a data
 points equal to the window size
*/
bool AccPopulated          ;
bool GyroPopulated         ;
bool PressurePopulated     ;
bool TemperaturePopulated  ;
bool CustomPopulated  ;
bool VoltagePopulated;
bool MagPopulated;
bool RTDPopulated;
bool OrientPopulated;
bool CurrentPopulated;


bool AccInit   = false;
bool GyroInit  = false;
bool MagInit   = false;
bool PressureInit = false;
bool TemperatureInit  = false;
bool VoltageInit    = false;
bool RTDInit = false;
bool OrientInit = false;
bool CurrentInit = false;

bool CustomInit  = false;
int Custom1Init = false;
int Custom2Init = false;
int Custom3Init = false;

#endif // DATA_H


