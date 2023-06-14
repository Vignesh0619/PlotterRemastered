
#ifndef BASEPLOT_H
#define BASEPLOT_H

#include <QVector>
#include "qcustomplot.h"
#include <QSharedDataPointer>
class baseplot
{
public:
/*
  Variable that are used to keep track of no.of data points added.
  Once 100 data points are added to the plotdata their respective vector
  is cleared.
*/
  int DataCounter=0;
    
/*
  Variable that is used to count if data points equal to the size of the window
  has been added or not
*/
  int Population=0;
/*
Circular  buffer used to store the value of time.This buffer is used to
remove old data points when a new data point  is added.
*/
  QVector<double>PreviousTime;
  int PreviousTimeCounter=0;//pointer for the buffer
/*
 is set to true it indicates that the respective plot has been populated with a data
 points equal to the window size
*/
  bool Populated = false;

  bool Init = false ;// if true indicates that user wants to plot this data
  bool Started= false;// if true indicates that Plotting for this has started
  int  WindowSize;
    
  int RMSLength = 100;
};

class OneGraph: public baseplot
{
public:
    QCustomPlot*  Plot = nullptr;
    QString GraphName;
    QString XAxisLabel;
    QString YAxisLabel;
    QSharedPointer<QCPGraphDataContainer> GraphData =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    int ConversionFactor;

    double RMS = 0;
    QVector<double> MS ;
    double MSSum =0;
    int RMSCounter =0;


    void GraphParameters(QString Name, QString XLabel,QString YLabel, int Factor, int Size, int Length);

    void GraphParameters(QString Name1,QString Name2, QString XLabel,QString YLabel, int Factor, int Size , int Length);

    void SetupPlot();

    void ParseData(QVector<double> &SensorValue,int32_t temp);

    void PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData);

    void CalculateRMS(QVector<double> &SensorValue);

};

class TwoGraph: public baseplot
{
  public:
    QCustomPlot*  Plot = nullptr;
    QString Graph1Name;
    QString Graph2Name;
    QString XAxisLabel;
    QString YAxisLabel;
    QSharedPointer<QCPGraphDataContainer> Graph1Data =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> Graph2Data =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    int ConversionFactor;
    
    double RMS1 = 0;
    QVector<double> MS1 ;
    double MS1Sum =0;
    int RMSCounter1 =0;

    double RMS2 = 0;
    QVector<double> MS2 ;
    double MS2Sum =0;
    int RMSCounter2 =0;

    void GraphParameters(QString Name1,QString Name2, QString XLabel,QString YLabel, int Factor, int Size , int Length);

    void SetupPlot();

    void ParseData(QVector<double> &Sensor1Value,QVector<double> &Sensor2Value, int32_t temp1,int32_t temp2);

    void PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2);

    void CalculateRMS(QVector<double> &Sensor1Value,QVector<double> &Sensor2Value);

};

class ThreeGraph: public baseplot
{
  public:
    QCustomPlot*  Plot = nullptr;
    QString Graph1Name;
    QString Graph2Name;
    QString Graph3Name;
    QString XAxisLabel;
    QString YAxisLabel;
    QSharedPointer<QCPGraphDataContainer> Graph1Data =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> Graph2Data =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> Graph3Data =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    int ConversionFactor;

    double RMS1 = 0;
    QVector<double> MS1 ;
    double MS1Sum =0;
    int RMSCounter1 =0;

    double RMS2 = 0;
    QVector<double> MS2 ;
    double MS2Sum =0;
    int RMSCounter2 =0;

    double RMS3 = 0;
    QVector<double> MS3 ;
    double MS3Sum =0;
    int RMSCounter3 =0;

    void GraphParameters(QString Name1,QString Name2,QString Name3, QString XLabel,QString YLabel, int Factor, int Size , int Length);

    void SetupPlot();

    void ParseData(QVector<double> &Sensor1Value,QVector<double> &Sensor2Value,QVector<double> &Sensor3Value, int32_t temp1,int32_t temp2,int32_t temp3);
 
    void PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2,QVector<double>&YAxisData3);

    void CalculateRMS(QVector<double> &SensorValue1 , QVector<double> &SensorValue2, QVector<double> &SensorValue3);
};

class MasterPlot: public  baseplot
{
   public:
   
    QCustomPlot*  Plot = nullptr; //Pointer variable to point to the qcustomplot widget
    
    QString Graph1Name; // 1st Graph Name
    QString Graph2Name; // 2nd Graph Name
    QString Graph3Name; // 3rd Graph Name

    QString XAxisLabel; // X Axis Label
    QString YAxisLabel; // Y Axis Label

    // QSharedPOinter of the type QCPGraphDataCotainer 
    // to store to respective graphs data

    QSharedPointer<QCPGraphDataContainer> Graph1Data =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> Graph2Data =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> Graph3Data =  QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    int ConversionFactor;// Conversion factor to convert int to double
    

    double RMS1 = 0;     // RMS of value of data displayed by graph 1
    QVector<double> MS1 ;// Storing the Mean square value for recursive RMS calculation 
    double MS1Sum =0;    // mean square sum
    int RMSCounter1 =0;  // counter to remove old data points mean square value

    double RMS2 = 0;
    QVector<double> MS2 ;
    double MS2Sum =0;
    int RMSCounter2 =0;

    double RMS3 = 0;
    QVector<double> MS3 ;
    double MS3Sum =0;
    int RMSCounter3 =0;

    int RMSCalcCounter=0;
    
   // Global Min & Max value of Graph1,2,3

    double GMinVal1=0;   
    double GMinVal2=0;
    double GMinVal3=0;

    double GMaxVal1=0;
    double GMaxVal2=0;
    double GMaxVal3=0;

   // Local Min and Max value of Graph 1,2,3

    double LMinVal1=0;
    double LMinVal2=0;
    double LMinVal3=0;

    double LMaxVal1=0;
    double LMaxVal2=0;
    double LMaxVal3=0;
    
    bool MinMax = false;
    int MinMaxCounter=0;
   
  /* 
   GraphParameters is a method that is called you have to assign names to 
   graphs,Label the axes, set the conversion factor of the sensor data,
   No.of Sample to be displayed and No.of Samples over which RMS has 
   to be calculated recursively
  */
    void GraphParameters(QString Name1, QString XLabel,QString YLabel, int Factor, int Size, int Length);
    void GraphParameters(QString Name1, QString Name2, QString XLabel,QString YLabel, int Factor, int Size, int Length);
    void GraphParameters(QString Name1, QString Name2, QString Name3, QString XLabel,QString YLabel, int Factor, int Size , int Length);
  /*
   GraphParameters that had been called before. These functions
   Setup1Graph and other function setup 1,2 or 3 graphs based on
   are called directly by graphparameters 
  */
    void Setup1Graph();
    void Setup2Graph();
    void Setup3Graph();

   //methods to parse the incoming data
    void ParseData(QVector<double> &SensorValue1,int32_t temp1);
    void ParseData(QVector<double> &SensorValue1,QVector<double> &SensorValue2, int32_t temp1,int32_t temp2);
    void ParseData(QVector<double> &SensorValue1,QVector<double> &SensorValue2,QVector<double> &SensorValue3, int32_t temp1,int32_t temp2,int32_t temp3);
 
  //methods to plot the data
    void PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1);
    void PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2);   
    void PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2,QVector<double>&YAxisData3);

  //methods to calculate RMS
    void CalculateRMS(QVector<double> &SensorValue1);
    void CalculateRMS(QVector<double> &SensorValue1 , QVector<double> &SensorValue2);
    void CalculateRMS(QVector<double> &SensorValue1 , QVector<double> &SensorValue2, QVector<double> &SensorValue3);
  /*
  Methods to calculate Global and Local Maxima, Minima along with RMS
  */
    void  
    void UpdateMinMax(QVector<double>&YAxisData1,QVector<double>&YAxisData2);
    void UpdateMinMax(QVector<double>&YAxisData1,QVector<double>&YAxisData2,QVector<double>&YAxisData3);
};
#endif // BASEPLOT_H
