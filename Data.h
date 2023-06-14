
#ifndef DATA_H
#define DATA_H

#include <QStringList>

typedef struct
{
    QVector<double> Time;
    QVector<double> AccX;
    QVector<double> AccY;
    QVector<double> AccZ;
    QVector<double> GyroX;
    QVector<double> GyroY;
    QVector<double> GyroZ;
    QVector<double> Pressure;
    QVector<double> Temperature;
    QVector<double> RTDTemp;
    QVector<double> MagX;
    QVector<double> MagY;
    QVector<double> MagZ;
    QVector<double> OrientX;
    QVector<double> OrientY;
    QVector<double> OrientZ;
    QVector<double> linear_acc_x;
    QVector<double> linear_acc_y;
    QVector<double> linear_acc_z;
    QVector<double> Current;
    QVector<double> BusVoltage;
    QVector<double> ShuntVoltage;
    QVector<double> AccCalib;
    QVector<double> GyroCalib;
    QVector<double> MagCalib;
    QVector<double> SysCalib;
    QVector<double> Counter;

} Sensor;
typedef struct
{
    uint32_t Time;
    int32_t AccX;
    int32_t AccY;
    int32_t AccZ;
    int32_t GyroX;
    int32_t GyroY;
    int32_t GyroZ;
    int32_t Pressure;
    int32_t Temperature;
    int32_t RTDTemp;
    int32_t MagX;
    int32_t MagY;
    int32_t MagZ;
    int32_t OrientX;
    int32_t OrientY;
    int32_t OrientZ;
    int32_t Current;
    int32_t Busvoltage;
    int32_t Shuntvoltage;
    uint8_t AccCalib;
    uint8_t GyroCalib;
    uint8_t MagCalib;
    uint8_t SysCalib;
    uint32_t Counter;

} Temp;
typedef struct
{
    QVector<double> CustomYAxisData1;
    QVector<double> CustomYAxisData2;
    QVector<double> CustomYAxisData3;

} Custom;

int windowsize; // =10000;      //Window size of the graph displyed

int Time_counter;

double TimerOffset; // stores the timer offset
bool TimerOffsetCalculated;

int CalibDataCounter;
    
bool StartedPlotting = false; // set to true once the user clicks start in the gui. True indicates that plotting has started

bool CustomInit = false;
int Custom1Init = false;
int Custom2Init = false;
int Custom3Init = false;

#endif // DATA_H
