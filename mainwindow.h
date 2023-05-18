
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QUdpsocket>
#include <QSharedDataPointer>
#include <QNetworkDatagram>
#include "qcustomplot.h"
#include <QMessageBox>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QUdpSocket *socket  = new QUdpSocket(this);

    QCustomPlot* PressurePlot = nullptr;
    QCustomPlot* TemperaturePlot = nullptr;
    QCustomPlot* VoltagePlot     = nullptr;
    QCustomPlot* AccPlot     = nullptr;
    QCustomPlot* GyroPlot     = nullptr;
    QCustomPlot* MagPlot     = nullptr;
    QCustomPlot* CustomPlot     = nullptr;
    QCustomPlot* RTDPlot     = nullptr;
    QCustomPlot* OrientPlot     = nullptr;
    QCustomPlot* CurrentPlot     = nullptr;

    QTimer *GraphTimer      =nullptr;
    QTimer  *RMSTimer     = nullptr;
    QTimer  *CalibTimer     = nullptr;

    void Setup1Plot(QCustomPlot* myPlot,QString GraphName, QString XAxisLabel, QString YAxisLabel
                   , QSharedPointer<QCPGraphDataContainer> PlotData);

    void Setup2Plot(QCustomPlot* myPlot,QString Graph1Name,QString Graph2Name, QString XAxisLabel, QString YAxisLabel
                    , QSharedPointer<QCPGraphDataContainer> Plot1Data, QSharedPointer<QCPGraphDataContainer> Plot2Data);

    void Setup3Plot(QCustomPlot* myPlot,QString Graph1Name,QString Graph2Name,QString Graph3Name, QString XAxisLabel, QString YAxisLabel
                    , QSharedPointer<QCPGraphDataContainer> Plot1Data, QSharedPointer<QCPGraphDataContainer> Plot2Data,
                                         QSharedPointer<QCPGraphDataContainer> Plot3Data);

    void Parse1Data(QVector<double> &SensorValue,uint32_t temp,int &DataCounter,int ConversionFactor);

    void Parse2Data(QVector<double> &SensorValue1,QVector<double> &SensorValue2,uint32_t temp1,uint32_t temp2,
                                                  int &DataCounter,int ConversionFactor1, int ConversionFactor2);

    void Parse3Data(QVector<double> &SensorValue1,QVector<double> &SensorValue2,QVector<double> &SensorValue3,
                    int32_t temp1,int32_t temp2,int32_t temp3, int &DataCounter,int ConversionFactor1, int ConversionFactor2,
                     int ConversionFactor3);

    void CalculateRMS(QVector <double>&SensorValue,QVector<double>&SensorValueMS,double &SensorValueMSSum, int &RMSCounter);

    void Plot1Data(QSharedPointer<QCPGraphDataContainer> PlotData,QVector<double>&XAxisData,QVector<double>&YAxisData
                   ,bool &Populated,int &Population,QVector<double>&PreviousTime,int &PreviousTimeCounter);

    void Plot2Data(QSharedPointer<QCPGraphDataContainer> Plot1Data,QSharedPointer<QCPGraphDataContainer> Plot2Data,
                   QVector<double>&XAxisData,QVector<double>&YAxisData1,QVector<double>&YAxisData2
                   ,bool &Populated,int &Population,QVector<double>&PreviousTime,int &PreviousTimeCounter);

    void Plot3Data(QSharedPointer<QCPGraphDataContainer> Plot1Data,QSharedPointer<QCPGraphDataContainer> Plot2Data,
                   QSharedPointer<QCPGraphDataContainer> Plot3Data,QVector<double>&XAxisData,QVector<double>&YAxisData1,
                   QVector<double>&YAxisData2,QVector<double>&YAxisData3,bool &Populated,
                                                 int &Population,QVector<double>&PreviousTime,int &PreviousTimeCounter);

    QSharedPointer<QCPGraphDataContainer> PressurePlotData    = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> TemperaturePlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> RTDPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> CurrentPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    QSharedPointer<QCPGraphDataContainer> BusVoltagePlotData  = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> ShuntVoltagePlotData= QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    QSharedPointer<QCPGraphDataContainer> AccXPlotData  = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> AccYPlotData  = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> AccZPlotData  = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    QSharedPointer<QCPGraphDataContainer> GyroXPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> GyroYPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> GyroZPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    QSharedPointer<QCPGraphDataContainer> MagXPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> MagYPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> MagZPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    QSharedPointer<QCPGraphDataContainer> OrientXPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> OrientYPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> OrientZPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    QSharedPointer<QCPGraphDataContainer> CustomPlotData1 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> CustomPlotData2 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> CustomPlotData3 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    int   YAxisCustom1=0;
    int   YAxisCustom2=0;
    int   YAxisCustom3=0;

    QString   YAxisCustomLabel1=0;
    QString   YAxisCustomLabel2=0;
    QString   YAxisCustomLabel3=0;

    QVector<QVector<double>> ParsedData;

    int PortNo = 2356;
private:
    Ui::MainWindow *ui;

    double RMSLength ;

    double PressureRMS = 0;
    QVector<double> PressureMS ;
    double PressureMSSum =0;
    int PressureRMSCounter =0;

    double TemperatureRMS = 0;
    QVector<double> TemperatureMS ;
    double TemperatureMSSum =0;
    int TemperatureRMSCounter =0;

    double RTDRMS = 0;
    QVector<double> RTDMS ;
    double RTDMSSum =0;
    int RTDRMSCounter =0;

    double BusVoltRMS = 0;
    QVector<double> BusVoltMS ;
    double BusVoltMSSum =0;
    int BusVoltRMSCounter =0;

    double ShuntVoltRMS = 0;
    QVector<double> ShuntVoltMS ;
    double ShuntVoltMSSum =0;
    int ShuntVoltRMSCounter =0;

    double CurrentRMS = 0;
    QVector<double> CurrentMS ;
    double CurrentMSSum =0;
    int CurrentRMSCounter =0;

signals:
    void NewDataRecieved();

public slots:
    void readyRead();
    void UpdateGraph();
    void UpdateCalibScore();
    void ParseNewData();
    void UpdateRMS();


private slots:
    void on_GyroBox_toggled(bool checked);
    void on_AccBox_toggled(bool checked);
    void on_TempBox_toggled(bool checked);
    void on_PressBox_toggled(bool checked);
    void on_CustomBox_toggled(bool checked);
    void on_VoltBox_toggled(bool checked);
    void on_RTDBox_toggled(bool checked);
    void on_MagBox_toggled(bool checked);
    void on_OrientBox_toggled(bool checked);
    void on_CurrentBox_toggled(bool checked);

    void on_WindowSizeLine_textChanged(const QString &arg1);
    void on_RMSLengthLine_textChanged(const QString &arg1);
    void on_PortNoLine_textChanged(const QString &arg1);

    void on_Start_clicked();

    void on_CustomY1_activated(int index);
    void on_CustomY2_activated(int index);
    void on_CustomY3_activated(int index);

    void on_CustomY1_textActivated(const QString &arg1);
    void on_CustomY2_textActivated(const QString &arg1);
    void on_CustomY3_textActivated(const QString &arg1);

    void on_CustomPlotData1_toggled(bool checked);
    void on_CustomPlotData12_toggled(bool checked);
    void on_CustomPlotData123_toggled(bool checked);




};

#endif // MAINWINDOW_H
