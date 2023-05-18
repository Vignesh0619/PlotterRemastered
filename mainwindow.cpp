
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Data.h"

Sensor SensorData;
Temp temp;
Custom CustomData;
bool Started  ;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    PressurePlot    = this->ui->PressurePlotWidget;
    TemperaturePlot = this->ui->TemperaturePlotWidget;
    VoltagePlot     = this->ui->VoltagePlotWidget;
    AccPlot         = this->ui->AccPlotWidget;
    GyroPlot        = this->ui->GyroPlotWidget;
    OrientPlot      = this->ui->OrientPlotWidget;
    CustomPlot      = this->ui->CustomPlotWidget;
    MagPlot         = this->ui->MagPlotWidget;
    RTDPlot         = this->ui->RTDPlotWidget;
    CurrentPlot     = this->ui->CurrentPlotWidget;

    ui->RMSLengthLine->setPlaceholderText("Enter a non-zero positive whole number as the RMSLength");
    ui->RMSLengthLine->setText("100");

    ui->WindowSizeLine->setPlaceholderText("Enter a non-zero positive whole number as the WindowSize");
    ui->WindowSizeLine->setText("10000");

    ui->IPAddrLine->setReadOnly(true);
    ui->IPAddrLine->setText("192.168.5.132");

    ui->PortNoLine->setText("2356");

    ui->PressureRMSLine->setReadOnly(true);
    ui->TemperatureRMSLine->setReadOnly(true);
    ui->RTDRMSLine->setReadOnly(true);
    ui->BusVoltRMSLine->setReadOnly(true);
    ui->ShuntVoltRMSLine->setReadOnly(true);

    QTimer *GraphTimer = new QTimer(this);
    GraphTimer->setInterval(500);
    connect(GraphTimer, SIGNAL(timeout()), this, SLOT(UpdateGraph()));

    QTimer *RMSTimer = new QTimer(this);
    RMSTimer->setInterval(200);
    connect(RMSTimer, SIGNAL(timeout()), this, SLOT(UpdateRMS()));

    QTimer *CalibTimer = new QTimer(this);
    CalibTimer->setInterval(250);
    connect(CalibTimer, SIGNAL(timeout()), this, SLOT(UpdateCalibScore()));


    GraphTimer->start();
    RMSTimer->start();
    CalibTimer->start();





    connect( this, &MainWindow::NewDataRecieved ,this , &MainWindow::ParseNewData );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Setup1Plot(QCustomPlot *myPlot, QString GraphName, QString XAxisLabel, QString YAxisLabel, QSharedPointer<QCPGraphDataContainer> PlotData)
{
    myPlot->addGraph();
    myPlot->graph(0)->setName(GraphName);
    myPlot->graph(0)->setPen(QPen(Qt::red));
    myPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    myPlot->graph(0)->setData(PlotData);
    
    myPlot->xAxis->setLabel(XAxisLabel);
    myPlot->yAxis->setLabel(YAxisLabel);

    myPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    myPlot->legend->setVisible(true);
    myPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    myPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
}

void MainWindow::Setup2Plot(QCustomPlot *myPlot, QString Graph1Name, QString Graph2Name, QString XAxisLabel, QString YAxisLabel, QSharedPointer<QCPGraphDataContainer> Plot1Data, QSharedPointer<QCPGraphDataContainer> Plot2Data)
{
  //graph start from zero but for readability purposes we have it as graph1 name graph2 name etc

    myPlot->addGraph();
    myPlot->graph(0)->setName(Graph1Name);
    myPlot->graph(0)->setPen(QPen(Qt::red));
    myPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    myPlot->graph(0)->setData(Plot1Data);

    myPlot->addGraph();
    myPlot->graph(1)->setName(Graph2Name);
    myPlot->graph(1)->setPen(QPen(Qt::blue));
    myPlot->graph(1)->setLineStyle(QCPGraph::lsLine);
    myPlot->graph(1)->setData(Plot2Data);

    myPlot->xAxis->setLabel(XAxisLabel);
    myPlot->yAxis->setLabel(YAxisLabel);

    myPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    myPlot->legend->setVisible(true);
    myPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    myPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

}

void MainWindow::Setup3Plot(QCustomPlot *myPlot, QString Graph1Name, QString Graph2Name, QString Graph3Name, QString XAxisLabel, QString YAxisLabel, QSharedPointer<QCPGraphDataContainer> Plot1Data, QSharedPointer<QCPGraphDataContainer> Plot2Data, QSharedPointer<QCPGraphDataContainer> Plot3Data)
{
    myPlot->addGraph();
    myPlot->graph(0)->setName(Graph1Name);
    myPlot->graph(0)->setPen(QPen(Qt::red));
    myPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    myPlot->graph(0)->setData(Plot1Data);

    myPlot->addGraph();
    myPlot->graph(1)->setName(Graph2Name);
    myPlot->graph(1)->setPen(QPen(Qt::blue));
    myPlot->graph(1)->setLineStyle(QCPGraph::lsLine);
    myPlot->graph(1)->setData(Plot2Data);

    myPlot->addGraph();
    myPlot->graph(2)->setName(Graph3Name);
    myPlot->graph(2)->setPen(QPen(Qt::green));
    myPlot->graph(2)->setLineStyle(QCPGraph::lsLine);
    myPlot->graph(2)->setData(Plot3Data);

    myPlot->xAxis->setLabel(XAxisLabel);
    myPlot->yAxis->setLabel(YAxisLabel);

    myPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    myPlot->legend->setVisible(true);
    myPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    myPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

}



void MainWindow::Parse1Data(QVector<double> &SensorValue, uint32_t temp, int &DataCounter, int ConversionFactor)
{

    if(DataCounter == 100)
    {
        DataCounter =0;
        SensorValue.clear();
    }
    SensorValue.append( ((double)temp)/ConversionFactor);
    DataCounter++;
}

void MainWindow::Parse2Data(QVector<double> &SensorValue1, QVector<double> &SensorValue2, uint32_t temp1,
                            uint32_t temp2, int &DataCounter, int ConversionFactor1, int ConversionFactor2)
{
    if(DataCounter == 100)
    {
        DataCounter =0;
        SensorValue1.clear();
        SensorValue2.clear();
    }
    SensorValue1.append( ((double)temp1)/ConversionFactor1); // dividing by 100000 to convert from Pa to bar
    SensorValue2.append( ((double)temp2)/ConversionFactor2); // dividing by 100000 to convert from Pa to bar
    DataCounter++;

}

void MainWindow::Parse3Data(QVector<double> &SensorValue1, QVector<double> &SensorValue2, QVector<double> &SensorValue3, int32_t temp1, int32_t temp2, int32_t temp3, int &DataCounter, int ConversionFactor1, int ConversionFactor2, int ConversionFactor3)
{
    if(DataCounter == 100)
    {
        DataCounter =0;
        SensorValue1.clear();
        SensorValue2.clear();
        SensorValue3.clear();
    }
    SensorValue1.append( ((double)temp1)/ConversionFactor1); // dividing by 100000 to convert from Pa to bar
    SensorValue2.append( ((double)temp2)/ConversionFactor2); // dividing by 100000 to convert from Pa to bar
    SensorValue3.append( ((double)temp3)/ConversionFactor3); // dividing by 100000 to convert from Pa to bar
    DataCounter++;
}



void MainWindow::Plot1Data(QSharedPointer<QCPGraphDataContainer> PlotData,QVector<double> &XAxisData, QVector<double> &YAxisData,
                           bool &Populated, int &Population, QVector<double> &PreviousTime, int &PreviousTimeCounter)
{
    if(Populated)
    {
        if(PreviousTimeCounter == windowsize)
            PreviousTimeCounter = 0;

        PlotData->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
        PreviousTimeCounter++;
        PlotData->add(QCPGraphData(XAxisData.last(),YAxisData.last()));
    }
    else
    {
        PlotData->add(QCPGraphData(XAxisData.last(),YAxisData.last()));
        PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
        PreviousTimeCounter++;
        Population++;
        if(Population >= windowsize)// check if we have 1000 data points in our graph if we do start clearing the oldest value
            Populated=true;
    }
}

void MainWindow::Plot2Data(QSharedPointer<QCPGraphDataContainer> Plot1Data, QSharedPointer<QCPGraphDataContainer> Plot2Data, QVector<double> &XAxisData, QVector<double> &YAxisData1, QVector<double> &YAxisData2, bool &Populated, int &Population, QVector<double> &PreviousTime, int &PreviousTimeCounter)
{
    if(Populated)
    {
        if(PreviousTimeCounter == windowsize)
            PreviousTimeCounter = 0;

        Plot1Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        Plot2Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
        PreviousTimeCounter++;
        Plot1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
        Plot2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
    }
    else
    {
        Plot1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
        Plot2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
        PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
        PreviousTimeCounter++;
        Population++;
        if(Population >= windowsize)// check if we have 1000 data points in our graph if we do start clearing the oldest value
            Populated=true;
    }
}

void MainWindow::Plot3Data(QSharedPointer<QCPGraphDataContainer> Plot1Data, QSharedPointer<QCPGraphDataContainer> Plot2Data, QSharedPointer<QCPGraphDataContainer> Plot3Data, QVector<double> &XAxisData, QVector<double> &YAxisData1, QVector<double> &YAxisData2, QVector<double> &YAxisData3, bool &Populated, int &Population, QVector<double> &PreviousTime, int &PreviousTimeCounter)
{
    if(Populated)
    {
        if(PreviousTimeCounter == windowsize)
            PreviousTimeCounter = 0;

        Plot1Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        Plot2Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        Plot3Data->remove(PreviousTime.at(PreviousTimeCounter));// this is done to remove old data points when a new data points is availale
        PreviousTime.replace(PreviousTimeCounter,XAxisData.last());// replace the oldest value in array with the latest value
        PreviousTimeCounter++;
        Plot1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
        Plot2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
        Plot3Data->add(QCPGraphData(XAxisData.last(),YAxisData3.last()));
    }
    else
    {
        Plot1Data->add(QCPGraphData(XAxisData.last(),YAxisData1.last()));
        Plot2Data->add(QCPGraphData(XAxisData.last(),YAxisData2.last()));
        Plot3Data->add(QCPGraphData(XAxisData.last(),YAxisData3.last()));
        PreviousTime.append(XAxisData.last());  // PreviousTime is a array to store to value
        PreviousTimeCounter++;
        Population++;
        if(Population >= windowsize)// check if we have 1000 data points in our graph if we do start clearing the oldest value
            Populated=true;
    }
}



void MainWindow::CalculateRMS(QVector<double> &SensorValue, QVector<double> &SensorValueMS, double &SensorValueMSSum, int &RMSCounter)
{
    if(SensorValueMS.length() < 100 && SensorValue.length() !=0)
    {
        SensorValueMS.append( (SensorValue.last() * SensorValue.last()) /RMSLength );
        SensorValueMSSum +=SensorValueMS.last();
    }
    if(SensorValueMS.length() >= 100)
    {
        if(RMSCounter == 100)
            RMSCounter=0;
        SensorValueMSSum= SensorValueMSSum - SensorValueMS.at(RMSCounter)+ (qPow(SensorValue.last(),2)  / RMSLength);
        SensorValueMS.replace(RMSCounter,(qPow(SensorValue.last(),2)  / RMSLength));
        RMSCounter++;
    }
}




void MainWindow::readyRead()
{
    QNetworkDatagram datagram = socket->receiveDatagram();

    if(datagram.data().count()) {
        memcpy(&temp.Time, datagram.data(), sizeof(temp));
    }

    emit NewDataRecieved();
}

void MainWindow::ParseNewData()
{
    //
    //Parsing the Calibration score data
    //
    if(CalibDataCounter >= 100)
    {
        CalibDataCounter=0;
        SensorData.SysCalib.append((int)temp.AccCalib);
        SensorData.AccCalib.append((int)temp.AccCalib);
        SensorData.GyroCalib.append((int)temp.GyroCalib);
        SensorData.MagCalib.append((int)temp.MagCalib);
        SensorData.SysCalib.clear();
        SensorData.AccCalib.clear();
        SensorData.GyroCalib.clear();
        SensorData.MagCalib.clear();
    }
    CalibDataCounter=0;
    SensorData.SysCalib.append((int)temp.AccCalib);
    SensorData.AccCalib.append((int)temp.AccCalib);
    SensorData.GyroCalib.append((int)temp.GyroCalib);
    SensorData.MagCalib.append((int)temp.MagCalib);
    //
    //Calculating the timer offset. Offset is calculate once only
    //
    if(!TimerOffsetCalculated){

        TimerOffset = (double)temp.Time;
        TimerOffsetCalculated= true;
    }

    //
    //Calculating time stamp value
    //
    if(TimerOffsetCalculated)
    {
        if(Time_counter == 100)
        {
            SensorData.Time.clear();
            Time_counter =0;
        }
        SensorData.Time.append( ( ( (double)temp.Time) - TimerOffset ) / 1000.0 ); //converting incoming milli sec data to secs
        Time_counter++;
    }

    if(AccInit || CustomInit)
    {
        Parse3Data(SensorData.AccX,SensorData.AccY,SensorData.AccZ,temp.AccX,temp.AccY,temp.AccZ,AccDataCounter,100,100,100);
        if(AccInit)
        Plot3Data(AccXPlotData,AccYPlotData,AccZPlotData,SensorData.Time,SensorData.AccX,SensorData.AccY,SensorData.AccZ,
                  AccPopulated,AccPopulation,AccPreviousTime,AccPreviousTimeCounter);

    }

    if(GyroInit||CustomInit)
    {
        Parse3Data(SensorData.GyroX,SensorData.GyroY,SensorData.GyroZ,temp.GyroX,temp.GyroY,temp.GyroZ,GyroDataCounter,100,100,100);
        if(GyroInit)
        Plot3Data(GyroXPlotData,GyroYPlotData,GyroZPlotData,SensorData.Time,SensorData.GyroX,SensorData.GyroY,SensorData.GyroZ,
                  GyroPopulated,GyroPopulation,GyroPreviousTime,GyroPreviousTimeCounter);

    }
    if(MagInit|| CustomInit)
    {
        Parse3Data(SensorData.MagX,SensorData.MagY,SensorData.MagZ,temp.MagX,temp.MagY,temp.MagZ,MagDataCounter,100,100,100);
        if(MagInit)
        Plot3Data(MagXPlotData,MagYPlotData,MagZPlotData,SensorData.Time,SensorData.MagX,SensorData.MagY,SensorData.MagZ,MagPopulated,
                  MagPopulation,MagPreviousTime,MagPreviousTimeCounter);
    }
    if(OrientInit||CustomInit)
    {
        Parse3Data(SensorData.OrientX,SensorData.OrientY,SensorData.OrientZ,temp.OrientX,temp.OrientY,temp.OrientZ,OrientDataCounter,10,10,10);
        if(OrientInit)
        Plot3Data(OrientXPlotData,OrientYPlotData,OrientZPlotData,SensorData.Time,SensorData.OrientX,SensorData.OrientY,SensorData.OrientZ,
                  OrientPopulated,OrientPopulation,OrientPreviousTime,OrientPreviousTimeCounter);
    }
    if(VoltageInit || CustomInit)
    {
        Parse2Data(SensorData.BusVoltage,SensorData.ShuntVoltage,temp.Busvoltage,temp.Shuntvoltage,VoltageDataCounter,100,100);
        if(VoltageInit)
        {
        CalculateRMS(SensorData.BusVoltage,BusVoltMS,BusVoltMSSum,BusVoltRMSCounter);
        CalculateRMS(SensorData.ShuntVoltage,ShuntVoltMS,ShuntVoltMSSum,ShuntVoltRMSCounter);
        Plot2Data(BusVoltagePlotData,ShuntVoltagePlotData,SensorData.Time,SensorData.BusVoltage,SensorData.ShuntVoltage,VoltagePopulated,
                  VoltagePopulation,VoltagePreviousTime,VoltagePreviousTimeCounter);
        }
    }
    if(PressureInit|| CustomInit)
    {
        Parse1Data(SensorData.Pressure,temp.Pressure,PressureDataCounter,100000);
        if(PressureInit)
        {
        CalculateRMS(SensorData.Pressure,PressureMS,PressureMSSum,PressureRMSCounter);
        Plot1Data(PressurePlotData,SensorData.Time,SensorData.Pressure,PressurePopulated,
                  PressurePopulation, PressurePreviousTime,PressurePreviousTimeCounter);
        }
    }
    if(TemperatureInit || CustomInit)
    {
        Parse1Data(SensorData.Temperature,temp.Temperature,TemperatureDataCounter,10);
        if(TemperatureInit)
        {
        CalculateRMS(SensorData.Temperature,TemperatureMS,TemperatureMSSum,TemperatureRMSCounter);
        Plot1Data(TemperaturePlotData,SensorData.Time,SensorData.Temperature,TemperaturePopulated,
                  TemperaturePopulation, TemperaturePreviousTime,TemperaturePreviousTimeCounter);
        }
    }
    if(RTDInit && temp.Counter >= 100 || CustomInit)
    {
        Parse1Data(SensorData.RTDTemp,temp.RTDTemp,RTDDataCounter,10);
        if(RTDInit)
        {
        Plot1Data(RTDPlotData,SensorData.Time,SensorData.RTDTemp,RTDPopulated,RTDPopulation,RTDPreviousTime,RTDPreviousTimeCounter);
        CalculateRMS(SensorData.RTDTemp,RTDMS,RTDMSSum,RTDRMSCounter);
        }
    }
    if(CurrentInit || CustomInit)
    {
        Parse1Data(SensorData.Current,temp.Current,CurrentDataCounter,10);
        if(CurrentInit)
        {
        Plot1Data(CurrentPlotData,SensorData.Time,SensorData.Current,CurrentPopulated,CurrentPopulation,CurrentPreviousTime,CurrentPreviousTimeCounter);
        CalculateRMS(SensorData.Current,CurrentMS,CurrentMSSum,CurrentRMSCounter);
        }
    }
    if(CustomInit)
    {
        ParsedData.insert(0,SensorData.AccX);
        ParsedData.insert(1,SensorData.AccY);
        ParsedData.insert(2,SensorData.AccZ);
        ParsedData.insert(3,SensorData.GyroX);
        ParsedData.insert(4,SensorData.GyroY);
        ParsedData.insert(5,SensorData.GyroZ);
        ParsedData.insert(6,(SensorData.Pressure));
        ParsedData.insert(7,(SensorData.Temperature));

        if(Custom3Init)
        {
         memcpy( &CustomData.YAxisData1,&ParsedData.at(YAxisCustom1), sizeof(ParsedData.at(YAxisCustom1)));
         memcpy( &CustomData.YAxisData2,&ParsedData.at(YAxisCustom2), sizeof(ParsedData.at(YAxisCustom2)));
         memcpy( &CustomData.YAxisData3,&ParsedData.at(YAxisCustom3), sizeof(ParsedData.at(YAxisCustom3)));

         Plot3Data(CustomPlotData1,CustomPlotData2,CustomPlotData3,SensorData.Time,CustomData.YAxisData1,CustomData.YAxisData2,CustomData.YAxisData3,
                  CustomPopulated,CustomPopulation,CustomPreviousTime,CustomPreviousTimeCounter);
        }
        if(Custom2Init)
        {
         memcpy( &CustomData.YAxisData1,&ParsedData.at(YAxisCustom1), sizeof(ParsedData.at(YAxisCustom1)));
         memcpy( &CustomData.YAxisData2,&ParsedData.at(YAxisCustom2), sizeof(ParsedData.at(YAxisCustom2)));
         Plot2Data(CustomPlotData1,CustomPlotData2,SensorData.Time,CustomData.YAxisData1,CustomData.YAxisData2,CustomPopulated,
                  CustomPopulation,CustomPreviousTime,CustomPreviousTimeCounter);
        }
        if(Custom1Init)
        {
         memcpy( &CustomData.YAxisData1,&ParsedData.at(YAxisCustom1), sizeof(ParsedData.at(YAxisCustom1)));
         Plot1Data(CustomPlotData1,SensorData.Time,CustomData.YAxisData1,CustomPopulated,CustomPopulation,CustomPreviousTime,CustomPreviousTimeCounter);
        }

    }

}



/*
  This is the function which is called whenever graphtimer is timed out.
  Although the sensor data is added to their respective graphs everytime a
  new data is recieved. The plot is only updated after after 500ms.
  The rate of update can be increased by changing the interval of graph
  timer. It is advised to not decrease the timer value below 500ms.
*/
void MainWindow::UpdateGraph()
{
    if(AccInit)
    {
        AccPlot->rescaleAxes();
        AccPlot->replot();
        AccPlot->update();
    }
    if(GyroInit)
    {
        GyroPlot->rescaleAxes();
        GyroPlot->replot();
        GyroPlot->update();
    }
    if(MagInit)
    {
        MagPlot->rescaleAxes();
        MagPlot->replot();
        MagPlot->update();
    }
    if(TemperatureInit)
    {
        TemperaturePlot->rescaleAxes();
        TemperaturePlot->replot();
        TemperaturePlot->update();
    }
    if(PressureInit)
    {
        PressurePlot->rescaleAxes();
        PressurePlot->replot();
        PressurePlot->update();
    }
    if(CustomInit)
    {
        CustomPlot->rescaleAxes();
        CustomPlot->replot();
        CustomPlot->update();
    }
    if(VoltageInit)
    {
        VoltagePlot->rescaleAxes();
        VoltagePlot->replot();
        VoltagePlot->update();
    }
    if(RTDInit)
    {
        RTDPlot->rescaleAxes();
        RTDPlot->replot();
        RTDPlot->update();
    }
    if(OrientInit)
    {
        OrientPlot->rescaleAxes();
        OrientPlot->replot();
        OrientPlot->update();
    }
    if(CurrentInit)
    {
        CurrentPlot->rescaleAxes();
        CurrentPlot->replot();
        CurrentPlot->update();
    }

}
/*
  This is the function that is called when RMSTime is timed out.
  This fucntion updates the RMS value displayed in the text box
  every 200ms. Rate of update can be increased. The value can be
  decreased but user might not be able to see the value getting
  updated if it decreased.
*/

void MainWindow::UpdateRMS()
{
    if(PressureMS.length() == 100)
    {
        PressureRMS=sqrt(PressureMSSum);
        ui->PressureRMSLine->setText(QString::number(PressureRMS));
    }
    if(TemperatureMS.length() == 100)
    {
        TemperatureRMS=sqrt(TemperatureMSSum);
        ui->TemperatureRMSLine->setText(QString::number(TemperatureRMS));
    }
    if(RTDMS.length() == 100)
    {
        RTDRMS=sqrt(RTDMSSum);
        ui->RTDRMSLine->setText(QString::number(RTDRMS));
    }
    if(BusVoltMS.length() == 100)
    {
        BusVoltRMS=sqrt(BusVoltMSSum);
        ui->BusVoltRMSLine->setText(QString::number(BusVoltRMS));
    }
    if(ShuntVoltMS.length() == 100)
    {
        ShuntVoltRMS=sqrt(ShuntVoltMSSum);
        ui->ShuntVoltRMSLine->setText(QString::number(ShuntVoltRMS));
    }
    if(CurrentMS.length() == 100)
    {
        CurrentRMS=sqrt(ShuntVoltMSSum);
        ui->CurrentRMSLine->setText(QString::number(CurrentRMS));
    }
}

void MainWindow::UpdateCalibScore()
{
    if(Started && SensorData.SysCalib.length() > 0)
    {
        ui->AccelCalibLine->setText(QString::number(SensorData.AccCalib.last()));
        ui->GyroCalibLine->setText(QString::number(SensorData.GyroCalib.last()));
        ui->MagCalibLine->setText(QString::number(SensorData.MagCalib.last()));
        ui->SysCalibLine->setText(QString::number(SensorData.SysCalib.last()));
    }
}

/*
 Slots to get user input of which data to plot
*/
void MainWindow::on_GyroBox_toggled(bool checked)
{
    GyroInit = checked;
}

void MainWindow::on_AccBox_toggled(bool checked)
{
    AccInit = checked;
}

void MainWindow::on_TempBox_toggled(bool checked)
{
    TemperatureInit = checked;
}

void MainWindow::on_PressBox_toggled(bool checked)
{
    PressureInit = checked;
}

void MainWindow::on_CustomBox_toggled(bool checked)
{
    CustomInit=checked;
}

void MainWindow::on_VoltBox_toggled(bool checked)
{
    VoltageInit = checked;
}

void MainWindow::on_MagBox_toggled(bool checked)
{
    MagInit = checked;
}

void MainWindow::on_RTDBox_toggled(bool checked)
{
    RTDInit = checked;
}
void MainWindow::on_OrientBox_toggled(bool checked)
{
    OrientInit = checked;
}

void MainWindow::on_CurrentBox_toggled(bool checked)
{
    CurrentInit = checked;
}


/*
 Slots to get the Windowsize of graph andRMS length and Port No.
*/
void MainWindow::on_WindowSizeLine_textChanged(const QString &arg1)
{
    windowsize = arg1.toInt();
}

void MainWindow::on_RMSLengthLine_textChanged(const QString &arg1)
{
    RMSLength =arg1.toInt();
}

void MainWindow::on_PortNoLine_textChanged(const QString &arg1)
{
    PortNo = arg1.toInt();
}


void MainWindow::on_Start_clicked()
{
    if(Started && windowsize > 0 && RMSLength > 0){
        QMessageBox *RebindMsgBox = new QMessageBox(this);
        RebindMsgBox->setIcon( QMessageBox::Warning );
        RebindMsgBox->setText("Plotting has started!!!!!!!");
        RebindMsgBox->setDetailedText("Ploter has started to plot the data switch to the respective tab to view the plot");
        QPushButton *btnCancel =  RebindMsgBox->addButton( "OK", QMessageBox::RejectRole );
        RebindMsgBox->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
        RebindMsgBox->exec();
    }
    if((!Started ) & (windowsize > 0) & (RMSLength > 0) & (Custom1Init+Custom2Init+Custom3Init > 1 ))
    {
        QMessageBox *InvalidMsgBox = new QMessageBox(this);
        InvalidMsgBox->setIcon( QMessageBox::Critical );
        InvalidMsgBox->setText("Invalid Custom Data Plot Selection");
        InvalidMsgBox->setDetailedText("Choose either CustomPlot Data1 or CustomPlot Data1 and Data2 or CustomPlot Data 1,2 and 3 ");
        QPushButton *btnOk =  InvalidMsgBox->addButton( "OK", QMessageBox::RejectRole );
        InvalidMsgBox->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
        InvalidMsgBox->show();
    }
    if((!Started ) & (windowsize > 0) & (RMSLength > 0) & (Custom1Init+Custom2Init+Custom3Init <= 1 ))
   {
    if(AccInit)
        Setup3Plot(AccPlot,"Acceleration along X","Acceleration along Y","Acceleration along Z","Time in secs","Acceleration in m/s^2",
                   AccXPlotData,AccYPlotData,AccZPlotData);

    if(GyroInit)
        Setup3Plot(GyroPlot,"Rotation along X","Rotation along Y","Rotation along Z","Time in secs","Rotation  in deg/s",
                   GyroXPlotData,GyroYPlotData,GyroZPlotData);
    if(MagInit)
        Setup3Plot(MagPlot,"Magnetic field strength along X","Magnetic field strength along Y","Magnetic field strength along Z",
                   "Time in secs","Magnetic field strength  in uT",MagXPlotData,MagYPlotData,MagZPlotData);
    if(OrientInit)
        Setup3Plot(OrientPlot,"Orientation wrt X","Orientation wrt Y","Orientation wrt Z","Time in secs",
                   "Orientation in degree",OrientXPlotData,OrientYPlotData,OrientZPlotData);
    if(PressureInit)
        Setup1Plot(PressurePlot,"Ambient Pressure","Time in secs","Pressure in bar",PressurePlotData);

    if(TemperatureInit)
        Setup1Plot(TemperaturePlot,"Ambient Temperature","Time in secs","Temperature in  celsius",TemperaturePlotData);

    if(VoltageInit)
        Setup2Plot(VoltagePlot,"Bus Voltage","Shunt Voltage","Time in secs","Voltage in  volts",BusVoltagePlotData,ShuntVoltagePlotData);
    if(RTDInit)
        Setup1Plot(RTDPlot," External Probe Temperature","Time in secs","Temperature in celsius",RTDPlotData);
    if(CurrentInit)
        Setup1Plot(CurrentPlot,"Current","Time in secs","Current in amps",CurrentPlotData);
    if(CustomInit)
    {
        if(Custom1Init)
            Setup1Plot(CustomPlot,YAxisCustomLabel1,"Time in secs","",CustomPlotData1);
        if(Custom2Init)
            Setup2Plot(CustomPlot,YAxisCustomLabel1,YAxisCustomLabel2,"Time in secs","",CustomPlotData1,CustomPlotData2);
        if(Custom3Init)
            Setup3Plot(CustomPlot,YAxisCustomLabel1,YAxisCustomLabel2,YAxisCustomLabel3,"Time in secs","",CustomPlotData1,CustomPlotData2,CustomPlotData3);
    }
    socket->bind(PortNo);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    Started = true;
    QMessageBox *StartedMsgBox = new QMessageBox(this);
    StartedMsgBox->setIcon( QMessageBox::NoIcon );
    StartedMsgBox->setText("Started Plotting!");
    QPushButton *btnOk =  StartedMsgBox->addButton( "OK", QMessageBox::RejectRole );
    StartedMsgBox->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
    StartedMsgBox->show();
   }
    if(windowsize <=0 || RMSLength <=0 )
    {
    QMessageBox *WarningMsgBox = new QMessageBox(this);
    WarningMsgBox->setIcon( QMessageBox::Critical );
    WarningMsgBox->setText("Window size and/or RMS length   has not been entered correctly!!");
    WarningMsgBox->setDetailedText("Enter a valid value for Window size and RMS Length ");
    QPushButton *btnOK =  WarningMsgBox->addButton( "OK", QMessageBox::RejectRole );
    WarningMsgBox->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
    WarningMsgBox->exec();
    }
    if(PortNo <= 0)
    {
    QMessageBox *PortMsgBox = new QMessageBox(this);
    PortMsgBox->setIcon( QMessageBox::Critical );
    PortMsgBox->setText("Invalid Port No");
    QPushButton *btnOk =  PortMsgBox->addButton( "OK", QMessageBox::RejectRole );
    PortMsgBox->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
    PortMsgBox->show();
    }
}


/*
  Getting the user input of what data he wants to plot
  as an Index number of that data in combo box as well as
  the option selected as string.Index is neccessary to
  pass that particular data to custom data strcut for plotting
  the string is required to add label to the graphs
*/
void MainWindow::on_CustomY1_activated(int index)
{
    YAxisCustom1 = index;
}

void MainWindow::on_CustomY2_activated(int index)
{
    YAxisCustom2 = index;
}

void MainWindow::on_CustomY3_activated(int index)
{
    YAxisCustom3 = index;
}


void MainWindow::on_CustomY1_textActivated(const QString &arg1)
{
    YAxisCustomLabel1= arg1;
}

void MainWindow::on_CustomY2_textActivated(const QString &arg1)
{
    YAxisCustomLabel2= arg1;
}
void MainWindow::on_CustomY3_textActivated(const QString &arg1)
{
    YAxisCustomLabel3= arg1;
}


/*
 getting user input of how many different sensor values
 have to be plotted
*/
void MainWindow::on_CustomPlotData1_toggled(bool checked)
{
    if(checked)
    Custom1Init=1;
    else
    Custom1Init =0;
}

void MainWindow::on_CustomPlotData12_toggled(bool checked)
{
    if(checked)
    Custom2Init =1;
    else
    Custom2Init =0;
}

void MainWindow::on_CustomPlotData123_toggled(bool checked)
{
    if(checked)
    Custom3Init =1;
    else
    Custom3Init =0;

}
