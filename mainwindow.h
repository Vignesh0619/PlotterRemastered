
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
#include "baseplot.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMessageBox *CustomMsgBox = nullptr;

    QUdpSocket *socket  = new QUdpSocket(this);

    QTimer *GraphTimer      =nullptr;
    QTimer  *RMSTimer     = nullptr;
    QTimer  *CalibTimer     = nullptr;


    QSharedPointer<QCPGraphDataContainer> RTDPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> CurrentPlotData = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    
    QSharedPointer<QCPGraphDataContainer> CustomPlotData1 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> CustomPlotData2 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    QSharedPointer<QCPGraphDataContainer> CustomPlotData3 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    int   YAxisCustom1=0;
    int   YAxisCustom2=0;
    int   YAxisCustom3=0;

    bool   YAxisCustom1Select=false;
    bool   YAxisCustom2Select=false;
    bool   YAxisCustom3Select=false;

    QString   YAxisCustomLabel1=0;
    QString   YAxisCustomLabel2=0;
    QString   YAxisCustomLabel3=0;

    QVector<QVector<double>> ParsedData={};

    int PortNo = 2198;

    MasterPlot PressureGraph , InternalTempGraph, ExternalTempGraph,CurrentGraph;
    MasterPlot VoltageGraph;
    MasterPlot AccGraph,GyroGraph,MagGraph,OrientGraph;
    MasterPlot CustomGraph;
    
    bool error= false;

    bool binded = false;
    bool Started  ;

    QVector<double> DefaultValue ={-1};

private:
    Ui::MainWindow *ui;

    double RMSLength ;


signals:
    void NewDataRecieved();

public slots:
    void readyRead();
    void UpdateGraph();
    void UpdateCalibScore();
    void ParseNewData();

private slots:
    void on_GyroBox_toggled(bool checked);
    void on_AccBox_toggled(bool checked);
    void on_TempBox_toggled(bool checked);
    void on_PressBox_toggled(bool checked);
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

    void on_comboBox_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
