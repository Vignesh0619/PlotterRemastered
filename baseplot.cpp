
#include "baseplot.h"



void OneGraph::SetupPlot()
{
    Plot->addGraph();
    Plot->graph(0)->setName(GraphName);
    Plot->graph(0)->setPen(QPen(Qt::red));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setData(GraphData);

    Plot->xAxis->setLabel(XAxisLabel);
    Plot->yAxis->setLabel(YAxisLabel);

    Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    Plot->legend->setVisible(true);
    Plot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    Plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
}

void OneGraph::GraphParameters(QString Name, QString XLabel, QString YLabel, int Factor, int Size, int Length)
{
    GraphName  = Name;
    XAxisLabel = XLabel;
    YAxisLabel = YLabel;
    ConversionFactor = Factor;
    WindowSize = Size;
    RMSLength =Length;
    Started=true;
    SetupPlot();
}

void OneGraph::ParseData(QVector<double> &SensorValue, int32_t temp)
{
    if(DataCounter == 100)
    {
        DataCounter =0;
        SensorValue.clear();
    }
    SensorValue.append( ((double)temp)/ConversionFactor);
    DataCounter++;
}

void OneGraph::PlotData(QVector<double> &XAxisData, QVector<double> &YAxisData)
{
    if(Populated)
    {
        if(PreviousTimeCounter == WindowSize)
            PreviousTimeCounter = 0;

        GraphData->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
        PreviousTimeCounter++;
        GraphData->add(QCPGraphData(XAxisData.last(),YAxisData.last()));
    }
    else
    {
        GraphData->add(QCPGraphData(XAxisData.last(),YAxisData.last()));
        PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
        PreviousTimeCounter++;
        Population++;
        if(Population >= WindowSize)// check if we have 1000 data points in our graph if we do start clearing the oldest value
            Populated=true;
    }
}

void OneGraph::CalculateRMS(QVector<double> &SensorValue)
{
    if(MS.length() < 100 && SensorValue.length() !=0)
    {
        MS.append( (SensorValue.last() * SensorValue.last()) /RMSLength );
        MSSum +=MS.last();
    }
    if(MS.length() >= 100)
    {
        if(RMSCounter == 100)
            RMSCounter=0;
        MSSum= MSSum - MS.at(RMSCounter)+ (qPow(SensorValue.last(),2)  / RMSLength);
        MS.replace(RMSCounter,(qPow(SensorValue.last(),2)  / RMSLength));
        RMSCounter++;
    }
}

/* definitions of TwoGraph methods*/

void TwoGraph::SetupPlot()
{
    Plot->addGraph();
    Plot->graph(0)->setName(Graph1Name);
    Plot->graph(0)->setPen(QPen(Qt::red));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setData(Graph1Data);

    Plot->addGraph();
    Plot->graph(1)->setName(Graph2Name);
    Plot->graph(1)->setPen(QPen(Qt::blue));
    Plot->graph(1)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(1)->setData(Graph2Data);

    Plot->xAxis->setLabel(XAxisLabel);
    Plot->yAxis->setLabel(YAxisLabel);

    Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    Plot->legend->setVisible(true);
    Plot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    Plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

}

void TwoGraph::GraphParameters(QString Name1, QString Name2, QString XLabel, QString YLabel, int Factor, int Size, int Length)
{
    Graph1Name  = Name1;
    Graph2Name  = Name2;
    XAxisLabel = XLabel;
    YAxisLabel = YLabel;
    ConversionFactor = Factor;
    WindowSize = Size;
    RMSLength = Length;
    Started=true;

    SetupPlot();
}

void TwoGraph::ParseData(QVector<double> &Sensor1Value,QVector<double> &Sensor2Value, int32_t temp1,int32_t temp2)
{
    if(DataCounter == 100)
    {
        DataCounter =0;
        Sensor1Value.clear();
        Sensor2Value.clear();
    }
    Sensor1Value.append( ((double)temp1)/ConversionFactor);
    Sensor2Value.append( ((double)temp2)/ConversionFactor);
    DataCounter++;
}

void TwoGraph::PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2)
{
    if(Populated)
    {
        if(PreviousTimeCounter == WindowSize)
            PreviousTimeCounter = 0;

        Graph1Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        Graph2Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
        PreviousTimeCounter++;
        Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
        Graph2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
    }
    else
    {
        Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
        Graph2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
        PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
        PreviousTimeCounter++;
        Population++;
        if(Population >= WindowSize)// check if we have 1000 data points in our graph if we do start clearing the oldest value
            Populated=true;
    }
}

void TwoGraph::CalculateRMS(QVector<double> &SensorValue1,QVector<double> &SensorValue2)
{
    if(MS1.length() < 100 && SensorValue1.length() !=0)
    {
        MS1.append( (SensorValue1.last() * SensorValue1.last()) /RMSLength );
        MS1Sum +=MS1.last();
    }
    if(MS1.length() >= 100)
    {
        if(RMSCounter1 == 100)
            RMSCounter1=0;
        MS1Sum= MS1Sum - MS1.at(RMSCounter1)+ (qPow(SensorValue1.last(),2)  / RMSLength);
        MS1.replace(RMSCounter1,(qPow(SensorValue1.last(),2)  / RMSLength));
        RMSCounter1++;
    }

    if(MS2.length() < 100 && SensorValue2.length() !=0)
    {
        MS2.append( (SensorValue2.last() * SensorValue2.last()) /RMSLength );
        MS2Sum +=MS2.last();
    }
    if(MS2.length() >= 100)
    {
        if(RMSCounter2 == 100)
            RMSCounter2=0;
        MS2Sum= MS2Sum - MS2.at(RMSCounter2)+ (qPow(SensorValue2.last(),2)  / RMSLength);
        MS2.replace(RMSCounter2,(qPow(SensorValue2.last(),2)  / RMSLength));
        RMSCounter2++;
    }

}
/* definition of ThreeGraph methods*/

void ThreeGraph::SetupPlot()
{
    Plot->addGraph();
    Plot->graph(0)->setName(Graph1Name);
    Plot->graph(0)->setPen(QPen(Qt::red));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setData(Graph1Data);

    Plot->addGraph();
    Plot->graph(1)->setName(Graph2Name);
    Plot->graph(1)->setPen(QPen(Qt::blue));
    Plot->graph(1)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(1)->setData(Graph2Data);

    Plot->addGraph();
    Plot->graph(2)->setName(Graph3Name);
    Plot->graph(2)->setPen(QPen(Qt::green));
    Plot->graph(2)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(2)->setData(Graph3Data);

    Plot->xAxis->setLabel(XAxisLabel);
    Plot->yAxis->setLabel(YAxisLabel);

    Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    Plot->legend->setVisible(true);
    Plot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    Plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
}

void ThreeGraph::GraphParameters(QString Name1, QString Name2, QString Name3, QString XLabel, QString YLabel, int Factor, int Size, int Length)
{
    Graph1Name  = Name1;
    Graph2Name  = Name2;
    Graph3Name  = Name3;
    XAxisLabel  = XLabel;
    YAxisLabel  = YLabel;
    ConversionFactor = Factor;
    WindowSize = Size;
    RMSLength =Length;
    Started=true;
    SetupPlot();
}

void ThreeGraph::ParseData(QVector<double> &Sensor1Value,QVector<double> &Sensor2Value,QVector<double> &Sensor3Value, int32_t temp1,int32_t temp2,int32_t temp3)
{
        if(DataCounter == 100)
    {
        DataCounter =0;
        Sensor1Value.clear();
        Sensor2Value.clear();
        Sensor3Value.clear();
    }
    Sensor1Value.append( ((double)temp1)/ConversionFactor); // dividing by 100000 to convert from Pa to bar
    Sensor2Value.append( ((double)temp2)/ConversionFactor); // dividing by 100000 to convert from Pa to bar
    Sensor3Value.append( ((double)temp3)/ConversionFactor); // dividing by 100000 to convert from Pa to bar
    DataCounter++;
}

void ThreeGraph::PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2,QVector<double>&YAxisData3)
{
    if(Populated)
    {
        if(PreviousTimeCounter == WindowSize)
            PreviousTimeCounter = 0;

        Graph1Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        Graph2Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        Graph3Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
        PreviousTimeCounter++;
        Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
        Graph2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
        Graph3Data->add(QCPGraphData(XAxisData.last(),YAxisData3.last()));
    }
    else
    {
        Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
        Graph2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
        Graph3Data->add(QCPGraphData(XAxisData.last(),YAxisData3.last()));
        PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
        PreviousTimeCounter++;
        Population++;
        if(Population >= WindowSize)// check if we have 1000 data points in our graph if we do start clearing the oldest value
            Populated=true;
    }
}

void ThreeGraph::CalculateRMS(QVector<double> &SensorValue1,QVector<double> &SensorValue2,QVector<double> &SensorValue3)
{
    if(MS1.length() < 100 && SensorValue1.length() !=0)
    {
        MS1.append( (SensorValue1.last() * SensorValue1.last()) /RMSLength );
        MS1Sum +=MS1.last();
    }
    if(MS1.length() >= 100)
    {
        if(RMSCounter1 == 100)
            RMSCounter1=0;
        MS1Sum= MS1Sum - MS1.at(RMSCounter1)+ (qPow(SensorValue1.last(),2)  / RMSLength);
        MS1.replace(RMSCounter1,(qPow(SensorValue1.last(),2)  / RMSLength));
        RMSCounter1++;
    }

    if(MS2.length() < 100 && SensorValue2.length() !=0)
    {
        MS2.append( (SensorValue2.last() * SensorValue2.last()) /RMSLength );
        MS2Sum +=MS2.last();
    }
    if(MS2.length() >= 100)
    {
        if(RMSCounter2 == 100)
            RMSCounter2=0;
        MS2Sum= MS2Sum - MS2.at(RMSCounter2)+ (qPow(SensorValue2.last(),2)  / RMSLength);
        MS2.replace(RMSCounter2,(qPow(SensorValue2.last(),2)  / RMSLength));
        RMSCounter2++;
    }

    if(MS3.length() < 100 && SensorValue3.length() !=0)
    {
        MS3.append( (SensorValue3.last() * SensorValue3.last()) /RMSLength );
        MS3Sum +=MS3.last();
    }
    if(MS3.length() >= 100)
    {
        if(RMSCounter3 == 100)
            RMSCounter3=0;
        MS3Sum= MS3Sum - MS3.at(RMSCounter3)+ (qPow(SensorValue3.last(),2)  / RMSLength);
        MS3.replace(RMSCounter3,(qPow(SensorValue3.last(),2)  / RMSLength));
        RMSCounter3++;
    }
}

/* definitions of MasterPlot methods*/

/* 
   GraphParameters is called you have to assign names to graphs,Label the axes,
   set the conversion factor of the sensor data,No.of Sample to be displayed
   and No.of Samples over which RMS has to be calculated recursively  
*/
void MasterPlot::GraphParameters(QString Name1, QString XLabel,QString YLabel, int Factor, int Size, int Length)
{
    Graph1Name  = Name1;
    XAxisLabel = XLabel;
    YAxisLabel = YLabel;
    ConversionFactor = Factor;
    WindowSize = Size;
    RMSLength = Length;
    Started=true;

    Setup1Graph();
}

void MasterPlot::GraphParameters(QString Name1, QString Name2, QString XLabel,QString YLabel, int Factor, int Size, int Length)
{
    Graph1Name  = Name1;
    Graph2Name  = Name2;
    XAxisLabel = XLabel;
    YAxisLabel = YLabel;
    ConversionFactor = Factor;
    WindowSize = Size;
    RMSLength = Length;
    Started=true;

    Setup2Graph();

}

void MasterPlot::GraphParameters(QString Name1, QString Name2, QString Name3, QString XLabel,QString YLabel, int Factor, int Size , int Length)
{
    Graph1Name  = Name1;
    Graph2Name  = Name2;
    Graph3Name  = Name3;
    XAxisLabel  = XLabel;
    YAxisLabel  = YLabel;
    ConversionFactor = Factor;
    WindowSize = Size;
    RMSLength =Length;
    Started=true;

    Setup3Graph();
}

/*
  Setup1Graph and other function setup 1,2 or 3 graphs based on
  GraphParameters that had been called before. These functions
  are called directly by graphparameters 
*/
void MasterPlot::Setup1Graph()
{
    Plot->addGraph();
    Plot->graph(0)->setName(Graph1Name);
    Plot->graph(0)->setPen(QPen(Qt::red));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setData(Graph1Data);

    Plot->xAxis->setLabel(XAxisLabel);
    Plot->yAxis->setLabel(YAxisLabel);

    Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    Plot->legend->setVisible(true);
    Plot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    Plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

}

void MasterPlot::Setup2Graph()
{
    Plot->addGraph();
    Plot->graph(0)->setName(Graph1Name);
    Plot->graph(0)->setPen(QPen(Qt::red));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setData(Graph1Data);

    Plot->addGraph();
    Plot->graph(1)->setName(Graph2Name);
    Plot->graph(1)->setPen(QPen(Qt::blue));
    Plot->graph(1)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(1)->setData(Graph2Data);

    Plot->xAxis->setLabel(XAxisLabel);
    Plot->yAxis->setLabel(YAxisLabel);

    Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    Plot->legend->setVisible(true);
    Plot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    Plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
}

void MasterPlot::Setup3Graph()
{
    Plot->addGraph();
    Plot->graph(0)->setName(Graph1Name);
    Plot->graph(0)->setPen(QPen(Qt::red));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setData(Graph1Data);

    Plot->addGraph();
    Plot->graph(1)->setName(Graph2Name);
    Plot->graph(1)->setPen(QPen(Qt::blue));
    Plot->graph(1)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(1)->setData(Graph2Data);

    Plot->addGraph();
    Plot->graph(2)->setName(Graph3Name);
    Plot->graph(2)->setPen(QPen(Qt::green));
    Plot->graph(2)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(2)->setData(Graph3Data);

    Plot->xAxis->setLabel(XAxisLabel);
    Plot->yAxis->setLabel(YAxisLabel);

    Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    Plot->legend->setVisible(true);
    Plot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    Plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
}

/*
  These functions Parse the incoming Data
*/
void MasterPlot::ParseData(QVector<double> &SensorValue1,int32_t temp1)
{
  if(DataCounter == 100)
    {
        DataCounter =0;
        SensorValue1.clear();
    }
    SensorValue1.append( ((double)temp1)/ConversionFactor); 
    DataCounter++;
}

void MasterPlot::ParseData(QVector<double> &SensorValue1,QVector<double> &SensorValue2,int32_t temp1,int32_t temp2)
{
  if(DataCounter == 100)
    {
        DataCounter =0;
        SensorValue1.clear();
        SensorValue2.clear();
        
    }
    SensorValue1.append( ((double)temp1)/ConversionFactor); // dividing by 100000 to convert from Pa to bar
    SensorValue2.append( ((double)temp2)/ConversionFactor); // dividing by 100000 to convert from Pa to bar
    DataCounter++;
}

void MasterPlot::ParseData(QVector<double> &SensorValue1,QVector<double> &SensorValue2,QVector<double> &SensorValue3, int32_t temp1,int32_t temp2,int32_t temp3)
{
  if(DataCounter == 100)
    {
        DataCounter =0;
        SensorValue1.clear();
        SensorValue2.clear();
        SensorValue3.clear();
    }
    SensorValue1.append( ((double)temp1)/ConversionFactor); // dividing by 100000 to convert from Pa to bar
    SensorValue2.append( ((double)temp2)/ConversionFactor); // dividing by 100000 to convert from Pa to bar
    SensorValue3.append( ((double)temp3)/ConversionFactor); // dividing by 100000 to convert from Pa to bar
    DataCounter++;
}

/*
   Functions to implement Running plot of the data
*/
void MasterPlot::PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1)
{
  if(Populated)
  {
    
    if(PreviousTimeCounter == WindowSize)
        PreviousTimeCounter = 0;  
    Graph1Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
    PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
    PreviousTimeCounter++;
    Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
  }
  else
  {  
    Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
    PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
    PreviousTimeCounter++;
    Population++;
    if(Population >= WindowSize)//check if we have data points equal to windowsize i.e. the graph samplesizr in our graph if we do start clearing the oldest value
        Populated=true;
  }
  UpdateMinMax(YAxisData1);
}

void MasterPlot::PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2)
{
  if(Populated)
  {
    if(PreviousTimeCounter == WindowSize)
        PreviousTimeCounter = 0; 
    Graph1Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
    Graph2Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
    PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
    PreviousTimeCounter++;
    Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
    Graph2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
  }
  else
  {
      Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
      Graph2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
      PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
      PreviousTimeCounter++;
      Population++;
      if(Population >= WindowSize)// check if we have 1000 data points in our graph if we do start clearing the oldest value
          Populated=true;
  }
  UpdateMinMax(YAxisData1,YAxisData2);
}

void MasterPlot::PlotData(QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2,QVector<double>&YAxisData3)
{
  if(Populated)
    {

      
      if(PreviousTimeCounter == WindowSize)
          PreviousTimeCounter = 0;
      
      Graph1Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
      Graph2Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
      Graph3Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
      PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
      PreviousTimeCounter++;
      Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
      Graph2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
      Graph3Data->add(QCPGraphData(XAxisData.last(),YAxisData3.last()));
    }
    else
    {

      Graph1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
      Graph2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
      Graph3Data->add(QCPGraphData(XAxisData.last(),YAxisData3.last()));
      PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
      PreviousTimeCounter++;
      Population++;
      if(Population >= WindowSize)// check if we have 1000 data points in our graph if we do start clearing the oldest value
          Populated=true;
    }
    UpdateMinMax(YAxisData1,YAxisData2,YAxisData3);
}

/*
 Functions to calculate RMS values of the sensor  data
*/
void MasterPlot::CalculateRMS(QVector<double> &SensorValue1)
{
    if(MS1.length() < 100 && SensorValue1.length() !=0)
    {
        MS1.append( (SensorValue1.last() * SensorValue1.last()) /RMSLength );
        MS1Sum +=MS1.last();
    }
    if(MS1.length() >= 100)
    {
        if(RMSCounter1 == 100)
            RMSCounter1=0;
        MS1Sum= MS1Sum - MS1.at(RMSCounter1)+ (qPow(SensorValue1.last(),2)  / RMSLength);
        MS1.replace(RMSCounter1,(qPow(SensorValue1.last(),2)  / RMSLength));
        RMSCounter1++;
    }
}

void MasterPlot::CalculateRMS(QVector<double> &SensorValue1 , QVector<double> &SensorValue2)
{
  if(MS1.length() < 100 && SensorValue1.length() !=0)
    {
        MS1.append( (SensorValue1.last() * SensorValue1.last()) /RMSLength );
        MS1Sum +=MS1.last();
    }
    if(MS1.length() >= 100)
    {
        if(RMSCounter1 == 100)
            RMSCounter1=0;
        MS1Sum= MS1Sum - MS1.at(RMSCounter1)+ (qPow(SensorValue1.last(),2)  / RMSLength);
        MS1.replace(RMSCounter1,(qPow(SensorValue1.last(),2)  / RMSLength));
        RMSCounter1++;
    }

    if(MS2.length() < 100 && SensorValue2.length() !=0)
    {
        MS2.append( (SensorValue2.last() * SensorValue2.last()) /RMSLength );
        MS2Sum +=MS2.last();
    }
    if(MS2.length() >= 100)
    {
        if(RMSCounter2 == 100)
            RMSCounter2=0;
        MS2Sum= MS2Sum - MS2.at(RMSCounter2)+ (qPow(SensorValue2.last(),2)  / RMSLength);
        MS2.replace(RMSCounter2,(qPow(SensorValue2.last(),2)  / RMSLength));
        RMSCounter2++;
    }
}
    
void MasterPlot::CalculateRMS(QVector<double> &SensorValue1 , QVector<double> &SensorValue2, QVector<double> &SensorValue3)
{
    if(MS1.length() < 100 && SensorValue1.length() !=0)
    {
        MS1.append( (SensorValue1.last() * SensorValue1.last()) /RMSLength );
        MS1Sum +=MS1.last();
    }
    if(MS1.length() >= 100)
    {
        if(RMSCounter1 == 100)
            RMSCounter1=0;
        MS1Sum= MS1Sum - MS1.at(RMSCounter1)+ (qPow(SensorValue1.last(),2)  / RMSLength);
        MS1.replace(RMSCounter1,(qPow(SensorValue1.last(),2)  / RMSLength));
        RMSCounter1++;
    }

    if(MS2.length() < 100 && SensorValue2.length() !=0)
    {
        MS2.append( (SensorValue2.last() * SensorValue2.last()) /RMSLength );
        MS2Sum +=MS2.last();
    }
    if(MS2.length() >= 100)
    {
        if(RMSCounter2 == 100)
            RMSCounter2=0;
        MS2Sum= MS2Sum - MS2.at(RMSCounter2)+ (qPow(SensorValue2.last(),2)  / RMSLength);
        MS2.replace(RMSCounter2,(qPow(SensorValue2.last(),2)  / RMSLength));
        RMSCounter2++;
    }

    if(MS3.length() < 100 && SensorValue3.length() !=0)
    {
        MS3.append( (SensorValue3.last() * SensorValue3.last()) /RMSLength );
        MS3Sum +=MS3.last();
    }
    if(MS3.length() >= 100)
    {
        if(RMSCounter3 == 100)
            RMSCounter3=0;
        MS3Sum= MS3Sum - MS3.at(RMSCounter3)+ (qPow(SensorValue3.last(),2)  / RMSLength);
        MS3.replace(RMSCounter3,(qPow(SensorValue3.last(),2)  / RMSLength));
        RMSCounter3++;
    }
}


void MasterPlot::UpdateMinMax(QVector<double>&YAxisData1,QVector<double>&YAxisData2,QVector<double>&YAxisData3)
{
 if(!MinMax)
    {
      GMaxVal1=YAxisData1.last();
      GMaxVal2=YAxisData2.last();
      GMaxVal3=YAxisData3.last();
      
      LMaxVal1=GMaxVal1;
      LMaxVal2=GMaxVal2;
      LMaxVal3=GMaxVal3;      

      GMinVal1=YAxisData1.last();
      GMinVal2=YAxisData2.last();
      GMinVal3=YAxisData3.last();  

      LMinVal1=GMinVal1;
      LMinVal2=GMinVal2;
      LMinVal3=GMinVal3;    
      
      MinMax=true;
    }

    GMaxVal1=(YAxisData1.last() > GMaxVal1) ? YAxisData1.last() : GMaxVal1 ;
    GMaxVal2=(YAxisData2.last() > GMaxVal2) ? YAxisData2.last() : GMaxVal2 ;
    GMaxVal3=(YAxisData3.last() > GMaxVal3) ? YAxisData3.last() : GMaxVal3 ;
    
    GMinVal1=(YAxisData1.last() < GMinVal1) ? YAxisData1.last() : GMinVal1 ;
    GMinVal2=(YAxisData2.last() < GMinVal2) ? YAxisData2.last() : GMinVal2 ;
    GMinVal3=(YAxisData3.last() < GMinVal3) ? YAxisData3.last() : GMinVal3 ;

    MinMaxCounter++;
    
    LMaxVal1= (YAxisData1.last() > LMaxVal1) ? YAxisData1.last() : LMaxVal1 ;
    LMaxVal2= (YAxisData2.last() > LMaxVal2) ? YAxisData2.last() : LMaxVal2 ;
    LMaxVal3= (YAxisData3.last() > LMaxVal3) ? YAxisData3.last() : LMaxVal3 ;
    
    LMinVal1= (YAxisData1.last() < LMinVal1) ? YAxisData1.last() : LMinVal1 ;
    LMinVal2= (YAxisData2.last() < LMinVal2) ? YAxisData2.last() : LMinVal2 ;
    LMinVal3= (YAxisData3.last() < LMinVal3) ? YAxisData3.last() : LMinVal3 ;

    if(MinMaxCounter % WindowSize == 0)
    {
      MinMaxCounter=0;
      LMinVal1=YAxisData1.last();
      LMinVal2=YAxisData2.last();
      LMinVal3=YAxisData3.last();
      LMaxVal1=YAxisData1.last();
      LMaxVal2=YAxisData2.last();
      LMaxVal3=YAxisData3.last();
    }  
//   if(RMSCalcCounter == 14)
//   { 
    if(MS1.length() < RMSLength && YAxisData1.length() !=0)
    {
        MS1.append( (YAxisData1.last() * YAxisData1.last()) /RMSLength );
        MS1Sum +=MS1.last();
    }
    if(MS1.length() >= RMSLength)
    {
        if(RMSCounter1 == RMSLength)
            RMSCounter1=0;
        MS1Sum= MS1Sum - MS1.at(RMSCounter1)+ (qPow(YAxisData1.last(),2)  / RMSLength);
        MS1.replace(RMSCounter1,(qPow(YAxisData1.last(),2)  / RMSLength));
        RMSCounter1++;
    }

    if(MS2.length() < RMSLength && YAxisData2.length() !=0)
    {
        MS2.append( (YAxisData2.last() * YAxisData2.last()) /RMSLength );
        MS2Sum +=MS2.last();
    }
    if(MS2.length() >= RMSLength)
    {
        if(RMSCounter2 == RMSLength)
            RMSCounter2=0;
        MS2Sum= MS2Sum - MS2.at(RMSCounter2)+ (qPow(YAxisData2.last(),2)  / RMSLength);
        MS2.replace(RMSCounter2,(qPow(YAxisData2.last(),2)  / RMSLength));
        RMSCounter2++;
    }

    if(MS3.length() < RMSLength && YAxisData3.length() !=0)
    {
        MS3.append( (YAxisData3.last() * YAxisData3.last()) /RMSLength );
        MS3Sum +=MS3.last();
    }
    if(MS3.length() >= RMSLength)
    {
        if(RMSCounter3 == RMSLength)
            RMSCounter3=0;
        MS3Sum= MS3Sum - MS3.at(RMSCounter3)+ (qPow(YAxisData3.last(),2)  / RMSLength);
        MS3.replace(RMSCounter3,(qPow(YAxisData3.last(),2)  / RMSLength));
        RMSCounter3++;
    }
    // RMSCalcCounter=0;
//    }
//    RMSCalcCounter++;


    Plot->graph(0)->setName(Graph1Name+" GMax\t"+QString::number(GMaxVal1,'f',2)+"\t"+"GMin\t"+QString::number(GMinVal1,'f',2)+"\t"
                                      +" LMax\t"+QString::number(LMaxVal1,'f',2)+"\t"+"LMin\t"+QString::number(LMinVal1,'f',2)+"\t"
                                      +" RMS\t" +QString::number(sqrt(MS1Sum),'f',2)  );

    Plot->graph(1)->setName(Graph2Name+" GMax\t"+QString::number(GMaxVal2,'f',2)+ "\t"+"GMin\t"+QString::number(GMinVal2,'f',2)+"\t"
                                      +" LMax\t"+QString::number(LMaxVal2,'f',2)+ "\t"+"LMin\t"+QString::number(LMinVal2,'f',2)+"\t"  
                                      +" RMS\t" +QString::number(sqrt(MS2Sum),'f',2)  ); 

    Plot->graph(2)->setName(Graph3Name+" GMax\t"+QString::number(GMaxVal3,'f',2)+ "\t"+"GMin\t"+QString::number(GMinVal3,'f',2)+"\t"   
                                      +" LMax\t"+QString::number(LMaxVal3,'f',2)+ "\t"+"LMin\t"+QString::number(LMinVal3,'f',2)+"\t"  
                                      +" RMS\t" +QString::number(sqrt(MS3Sum),'f',2)  );
}

void MasterPlot::UpdateMinMax(QVector<double>&YAxisData1,QVector<double>&YAxisData2)
{
  if(!MinMax)
    {
      GMaxVal1=YAxisData1.last();
      GMaxVal2=YAxisData2.last();
      
      LMaxVal1=GMaxVal1;
      LMaxVal2=GMaxVal2;

      GMinVal1=YAxisData1.last();
      GMinVal2=YAxisData2.last();

      LMinVal1=GMinVal1;
      LMinVal2=GMinVal2;
      
      MinMax=true;
    }

    GMaxVal1=(YAxisData1.last() > GMaxVal1) ? YAxisData1.last() : GMaxVal1 ;
    GMaxVal2=(YAxisData2.last() > GMaxVal2) ? YAxisData2.last() : GMaxVal2 ;
    
    GMinVal1=(YAxisData1.last() < GMinVal1) ? YAxisData1.last() : GMinVal1 ;
    GMinVal2=(YAxisData2.last() < GMinVal2) ? YAxisData2.last() : GMinVal2 ;

    MinMaxCounter++;
    
    LMaxVal1= (YAxisData1.last() > LMaxVal1) ? YAxisData1.last() : LMaxVal1 ;
    LMaxVal2= (YAxisData2.last() > LMaxVal2) ? YAxisData2.last() : LMaxVal2 ;
    
    LMinVal1= (YAxisData1.last() < LMinVal1) ? YAxisData1.last() : LMinVal1 ;
    LMinVal2= (YAxisData2.last() < LMinVal2) ? YAxisData2.last() : LMinVal2 ;

    if(MinMaxCounter % WindowSize == 0)
    {
      MinMaxCounter=0;
      LMinVal1=YAxisData1.last();
      LMinVal2=YAxisData2.last();
      LMaxVal1=YAxisData1.last();
      LMaxVal2=YAxisData2.last();
    }  
   if(RMSCalcCounter == 14)
   {
    if(MS1.length() < RMSLength && YAxisData1.length() !=0)
    {
        MS1.append( (YAxisData1.last() * YAxisData1.last()) /RMSLength );
        MS1Sum +=MS1.last();
    }
    if(MS1.length() >= RMSLength)
    {
        if(RMSCounter1 == RMSLength)
            RMSCounter1=0;
        MS1Sum= MS1Sum - MS1.at(RMSCounter1)+ (qPow(YAxisData1.last(),2)  / RMSLength);
        MS1.replace(RMSCounter1,(qPow(YAxisData1.last(),2)  / RMSLength));
        RMSCounter1++;
    }

    if(MS2.length() < RMSLength && YAxisData2.length() !=0)
    {
        MS2.append( (YAxisData2.last() * YAxisData2.last()) /RMSLength );
        MS2Sum +=MS2.last();
    }
    if(MS2.length() >= RMSLength)
    {
        if(RMSCounter2 == RMSLength)
            RMSCounter2=0;
        MS2Sum= MS2Sum - MS2.at(RMSCounter2)+ (qPow(YAxisData2.last(),2)  / RMSLength);
        MS2.replace(RMSCounter2,(qPow(YAxisData2.last(),2)  / RMSLength));
        RMSCounter2++;
    }
    RMSCalcCounter=0;
   }
   RMSCalcCounter++;
    Plot->graph(0)->setName(Graph1Name+" GMax "+QString::number(GMaxVal1)+" GMin "+QString::number(GMinVal1)
                                      +" LMax "+QString::number(LMaxVal1)+" LMin "+QString::number(LMinVal1)  
                                      +" RMS " +QString::number(sqrt(MS1Sum))  );

    Plot->graph(1)->setName(Graph2Name+" GMax "+QString::number(GMaxVal2)+" GMin "+QString::number(GMinVal2)
                                      +" LMax "+QString::number(LMaxVal2)+" LMin "+QString::number(LMinVal2)  
                                      +" RMS " +QString::number(sqrt(MS2Sum))  );

}

void MasterPlot::UpdateMinMax(QVector<double>&YAxisData1)
{
   if(!MinMax)
    {
      GMaxVal1=YAxisData1.last();
      GMinVal1=YAxisData1.last();

      LMaxVal1=GMaxVal1;
      LMinVal1=GMinVal1;
      
      MinMax=true;
    }

    GMaxVal1=(YAxisData1.last() > GMaxVal1) ? YAxisData1.last() : GMaxVal1 ;
    GMinVal1=(YAxisData1.last() < GMinVal1) ? YAxisData1.last() : GMinVal1 ;

    MinMaxCounter++;
    
    LMaxVal1= (YAxisData1.last() > LMaxVal1) ? YAxisData1.last() : LMaxVal1 ;  
    LMinVal1= (YAxisData1.last() < LMinVal1) ? YAxisData1.last() : LMinVal1 ;

    if(MinMaxCounter % WindowSize == 0)
    {
      MinMaxCounter=0;
      LMinVal1=YAxisData1.last();
      LMaxVal1=YAxisData1.last();
    }
      
   if(RMSCalcCounter == 14)
   {

     if(MS1.length() < RMSLength && YAxisData1.length() !=0)
    {
        MS1.append( (YAxisData1.last() * YAxisData1.last()) /RMSLength );
        MS1Sum +=MS1.last();
    }
    if(MS1.length() >= RMSLength)
    {
        if(RMSCounter1 == 100)
            RMSCounter1=0;
        MS1Sum= MS1Sum - MS1.at(RMSCounter1)+ (qPow(YAxisData1.last(),2)  / RMSLength);
        MS1.replace(RMSCounter1,(qPow(YAxisData1.last(),2)  / RMSLength));
        RMSCounter1++;
    }
      RMSCalcCounter==0;
   }
     RMSCalcCounter++;

    Plot->graph(0)->setName(Graph1Name+" GMax "+QString::number(GMaxVal1)+" GMin "+QString::number(GMinVal1)
                                      +" LMax "+QString::number(LMaxVal1)+" LMin "+QString::number(LMinVal1)
                                      +" RMS " +QString::number(sqrt(MS1Sum))  );

}
