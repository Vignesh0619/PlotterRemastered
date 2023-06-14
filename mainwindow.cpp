
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Data.h"

Sensor SensorData;
Temp temp;
Custom CustomData;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    CustomMsgBox = new QMessageBox(this);
    QPushButton *btnOk = CustomMsgBox->addButton("OK", QMessageBox::RejectRole);

    PressureGraph.Plot = this->ui->PressurePlotWidget;
    InternalTempGraph.Plot = this->ui->TemperaturePlotWidget;
    ExternalTempGraph.Plot = this->ui->RTDPlotWidget;
    CurrentGraph.Plot = this->ui->CurrentPlotWidget;

    VoltageGraph.Plot = this->ui->VoltagePlotWidget;

    AccGraph.Plot = this->ui->AccPlotWidget;
    GyroGraph.Plot = this->ui->GyroPlotWidget;
    OrientGraph.Plot = this->ui->OrientPlotWidget;
    MagGraph.Plot = this->ui->MagPlotWidget;

    CustomGraph.Plot = this->ui->CustomPlotWidget;

    ui->RMSLengthLine->setPlaceholderText("Enter a non-zero positive whole number as the RMSLength");
    ui->RMSLengthLine->setText("100");

    ui->WindowSizeLine->setPlaceholderText("Enter a non-zero positive whole number as the WindowSize");
    ui->WindowSizeLine->setText("500");

    ui->IPAddrLine->setReadOnly(true);
    ui->IPAddrLine->setText("192.168.5.132");

    ui->PortNoLine->setText("2198");

    QTimer *GraphTimer = new QTimer(this);
    GraphTimer->setInterval(50);
    connect(GraphTimer, SIGNAL(timeout()), this, SLOT(UpdateGraph()));

    QTimer *CalibTimer = new QTimer(this);
    CalibTimer->setInterval(250);
    connect(CalibTimer, SIGNAL(timeout()), this, SLOT(UpdateCalibScore()));

    GraphTimer->start();
    CalibTimer->start();

    // connect the slot which has to be called whenever new data is recieved
    connect(this, &MainWindow::NewDataRecieved, this, &MainWindow::ParseNewData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readyRead()
{
    QNetworkDatagram datagram = socket->receiveDatagram();
    if (datagram.data().count())
    {
        memcpy(&temp.Time, datagram.data(), sizeof(temp));
    }
    emit NewDataRecieved();
}

void MainWindow::ParseNewData()
{
 //
 // Parsing the Calibration score data
 //
 if (CalibDataCounter >= 100)
 {
     CalibDataCounter = 0;
     SensorData.SysCalib.append((int)temp.AccCalib);
     SensorData.AccCalib.append((int)temp.AccCalib);
     SensorData.GyroCalib.append((int)temp.GyroCalib);
     SensorData.MagCalib.append((int)temp.MagCalib);
     SensorData.SysCalib.clear();
     SensorData.AccCalib.clear();
     SensorData.GyroCalib.clear();
     SensorData.MagCalib.clear();
 }
 CalibDataCounter = 0;
 SensorData.SysCalib.append((int)temp.AccCalib);
 SensorData.AccCalib.append((int)temp.AccCalib);
 SensorData.GyroCalib.append((int)temp.GyroCalib);
 SensorData.MagCalib.append((int)temp.MagCalib);
 //
 // Calculating the timer offset. Offset is calculate once only
 //
 if (!TimerOffsetCalculated)
 { 
     TimerOffset = (double)temp.Time;
     TimerOffsetCalculated = true;
 } 
 //
 // Calculating time stamp value
 //
  if (TimerOffsetCalculated)
  {
      if (Time_counter == 100)
      {
          SensorData.Time.clear();
          Time_counter = 0;
      }
      SensorData.Time.append((((double)temp.Time) - TimerOffset) / 1000.0); // converting incoming milli sec data to secs
      Time_counter++;
  }
  if (AccGraph.Started || CustomGraph.Started)
  {
      AccGraph.ParseData(SensorData.AccX, SensorData.AccY, SensorData.AccZ, temp.AccX, temp.AccY, temp.AccZ);
      if (AccGraph.Started)
          AccGraph.PlotData(SensorData.Time, SensorData.AccX, SensorData.AccY, SensorData.AccZ);
  }
  if (GyroGraph.Started || CustomGraph.Started)
  {
      GyroGraph.ParseData(SensorData.GyroX, SensorData.GyroY, SensorData.GyroZ, temp.GyroX, temp.GyroY, temp.GyroZ);
      if (GyroGraph.Started)
          GyroGraph.PlotData(SensorData.Time, SensorData.GyroX, SensorData.GyroY, SensorData.GyroZ);
  }
  if (MagGraph.Started || CustomGraph.Started)
  {
      MagGraph.ParseData(SensorData.MagX, SensorData.MagY, SensorData.MagZ, temp.MagX, temp.MagY, temp.MagZ);
      if (MagGraph.Started)
          MagGraph.PlotData(SensorData.Time, SensorData.MagX, SensorData.MagY, SensorData.MagZ);
  }
  if (OrientGraph.Started || CustomGraph.Started)
  {
      OrientGraph.ParseData(SensorData.OrientX, SensorData.OrientY, SensorData.OrientZ, temp.OrientX, temp.OrientY, temp.OrientZ);
      if (OrientGraph.Started)
          OrientGraph.PlotData(SensorData.Time, SensorData.OrientX, SensorData.OrientY, SensorData.OrientZ);
  }
  if (VoltageGraph.Started || CustomGraph.Started)
  {
      VoltageGraph.ParseData(SensorData.BusVoltage, SensorData.ShuntVoltage, temp.Busvoltage, temp.Shuntvoltage);
      if (VoltageGraph.Started)
          VoltageGraph.PlotData(SensorData.Time, SensorData.BusVoltage, SensorData.ShuntVoltage);
  }
  if (PressureGraph.Started || CustomGraph.Started)
  {
      PressureGraph.ParseData(SensorData.Pressure, temp.Pressure);
      if (PressureGraph.Started)
          PressureGraph.PlotData(SensorData.Time, SensorData.Pressure);
  }
  if (InternalTempGraph.Started || CustomGraph.Started)
  {
      InternalTempGraph.ParseData(SensorData.Temperature, temp.Temperature);
      if (InternalTempGraph.Started)
          InternalTempGraph.PlotData(SensorData.Time, SensorData.Temperature);
  }
  if ((ExternalTempGraph.Started && (temp.Counter % 10 == 0)) || CustomGraph.Started)
  {
      ExternalTempGraph.ParseData(SensorData.RTDTemp, temp.RTDTemp);
      if (ExternalTempGraph.Started)
          ExternalTempGraph.PlotData(SensorData.Time, SensorData.RTDTemp);
  }
  if (CurrentGraph.Started || CustomGraph.Started)
  {
      CurrentGraph.ParseData(SensorData.Current, temp.Current);
      if (CurrentGraph.Started)
          CurrentGraph.PlotData(SensorData.Time, SensorData.Current);
  }
  if (CustomGraph.Started)
  {
      ParsedData.insert(0, DefaultValue);
      ParsedData.insert(1, SensorData.AccX);
      ParsedData.insert(2, SensorData.AccY);
      ParsedData.insert(3, SensorData.AccZ);
      ParsedData.insert(4, SensorData.GyroX);
      ParsedData.insert(5, SensorData.GyroY);
      ParsedData.insert(6, SensorData.GyroZ);
      ParsedData.insert(7, SensorData.MagX);
      ParsedData.insert(8, SensorData.MagY);
      ParsedData.insert(9, SensorData.MagZ);
      ParsedData.insert(10, SensorData.OrientX);
      ParsedData.insert(11, SensorData.OrientY);
      ParsedData.insert(12, SensorData.OrientZ);
      ParsedData.insert(13, SensorData.Pressure);
      ParsedData.insert(14, SensorData.Temperature);
      ParsedData.insert(15, SensorData.RTDTemp);
      ParsedData.insert(16, SensorData.Current);
      ParsedData.insert(17, SensorData.BusVoltage);
      ParsedData.insert(18, SensorData.ShuntVoltage);
      if (Custom3Init && CustomGraph.Started)
      {
          if (CustomGraph.DataCounter >= 100)
          {
              CustomData.CustomYAxisData3.clear();
              CustomData.CustomYAxisData2.clear();
              CustomData.CustomYAxisData1.clear();
              CustomGraph.DataCounter = 0;
          }
          CustomData.CustomYAxisData1.append((float)(ParsedData.at(YAxisCustom1)).last());
          CustomData.CustomYAxisData2.append((float)(ParsedData.at(YAxisCustom2)).last());
          CustomData.CustomYAxisData3.append((float)(ParsedData.at(YAxisCustom3)).last());
          CustomGraph.DataCounter++;
          CustomGraph.PlotData(SensorData.Time, CustomData.CustomYAxisData1, CustomData.CustomYAxisData2, CustomData.CustomYAxisData3);
      }
      if (Custom2Init && CustomGraph.Started)
      {
          CustomData.CustomYAxisData1.append((float)(ParsedData.at(YAxisCustom1)).last());
          CustomData.CustomYAxisData2.append((float)(ParsedData.at(YAxisCustom2)).last());
          CustomGraph.PlotData(SensorData.Time, CustomData.CustomYAxisData1, CustomData.CustomYAxisData2);
      }
      if (Custom1Init && CustomGraph.Started)
      {
          CustomData.CustomYAxisData1.append((float)(ParsedData.at(YAxisCustom1)).last());
          CustomGraph.PlotData(SensorData.Time, CustomData.CustomYAxisData1);
      }
      ParsedData.clear();
  }
}

/*
  This is the function which is called whenever graphtimer is timed out.
  Although the sensor data is added to their respective graphs everytime a
  new data is recieved. The plot is only updated after after 500ms.
  The rate of update can be increased by changing the interval of graph
  timer. It is advised to not decrease the timer value below 500ms.

  Also the updategraphs() have to be called whenever you want to change 
  something in the legend. For Eg when the Global Maxima is to changed
  then we have replot() and update() the plots using updategraphs()
*/
void MainWindow::UpdateGraph()
{
 if (AccGraph.Started)
 {
     AccGraph.Plot->rescaleAxes();
     AccGraph.Plot->replot();
     AccGraph.Plot->update();
 }
 if (GyroGraph.Started)
 {
     GyroGraph.Plot->rescaleAxes();
     GyroGraph.Plot->replot();
     GyroGraph.Plot->update();
 }
 if (MagGraph.Started)
 {
     MagGraph.Plot->rescaleAxes();
     MagGraph.Plot->replot();
     MagGraph.Plot->update();
 }
 if (InternalTempGraph.Started)
 {
     InternalTempGraph.Plot->rescaleAxes();
     InternalTempGraph.Plot->replot();
     InternalTempGraph.Plot->update();
 }
 if (PressureGraph.Started)
 {
     PressureGraph.Plot->rescaleAxes();
     PressureGraph.Plot->replot();
     PressureGraph.Plot->update();
 }
 if (CustomGraph.Started)
 {
     CustomGraph.Plot->rescaleAxes();
     CustomGraph.Plot->replot();
     CustomGraph.Plot->update();
 }
 if (VoltageGraph.Started)
 {
     VoltageGraph.Plot->rescaleAxes();
     VoltageGraph.Plot->replot();
     VoltageGraph.Plot->update();
 }
 if (ExternalTempGraph.Started)
 {
     ExternalTempGraph.Plot->rescaleAxes();
     ExternalTempGraph.Plot->replot();
     ExternalTempGraph.Plot->update();
 }
 if (OrientGraph.Started)
 {
     OrientGraph.Plot->rescaleAxes();
     OrientGraph.Plot->replot();
     OrientGraph.Plot->update();
 }
 if (CurrentGraph.Started)
 {
     CurrentGraph.Plot->rescaleAxes();
     CurrentGraph.Plot->replot();
     CurrentGraph.Plot->update();
 }
}

void MainWindow::UpdateCalibScore()
{
 if (SensorData.SysCalib.length() > 0)
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
    GyroGraph.Init = checked;
}

void MainWindow::on_AccBox_toggled(bool checked)
{
    AccGraph.Init = checked;
}

void MainWindow::on_TempBox_toggled(bool checked)
{
    InternalTempGraph.Init = checked;
}

void MainWindow::on_PressBox_toggled(bool checked)
{
    PressureGraph.Init = checked;
}

void MainWindow::on_VoltBox_toggled(bool checked)
{
    VoltageGraph.Init = checked;
}

void MainWindow::on_MagBox_toggled(bool checked)
{
    MagGraph.Init = checked;
}

void MainWindow::on_RTDBox_toggled(bool checked)
{
    ExternalTempGraph.Init = checked;
}
void MainWindow::on_OrientBox_toggled(bool checked)
{
    OrientGraph.Init = checked;
}

void MainWindow::on_CurrentBox_toggled(bool checked)
{
    CurrentGraph.Init = checked;
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
    RMSLength = arg1.toInt();
}

void MainWindow::on_PortNoLine_textChanged(const QString &arg1)
{
    PortNo = arg1.toInt();
}

void MainWindow::on_Start_clicked()
{

    if ((windowsize > 0) && (RMSLength > 0) && (Custom1Init + Custom2Init + Custom3Init > 1))
    {
        CustomMsgBox->setIcon(QMessageBox::Critical);
        CustomMsgBox->setText("Invalid Custom Data Plot Selection");
        CustomMsgBox->setDetailedText("Choose either CustomPlot Data1 or CustomPlot Data1 and Data2 or CustomPlot Data 1,2 and 3 ");
        CustomMsgBox->show();
    }

    if ((windowsize > 0) && (RMSLength > 0) && (Custom1Init + Custom2Init + Custom3Init <= 1))
    {
        if (AccGraph.Init && !AccGraph.Started)
            AccGraph.GraphParameters("X", "Y", "Z", "Time in secs", "Acceleration in m/s^2", 100, windowsize, RMSLength);

        if (GyroGraph.Init && !GyroGraph.Started)
            GyroGraph.GraphParameters("X", "Y", "Z", "Time in secs", "Rotation  in deg/s", 100, windowsize, RMSLength);

        if (MagGraph.Init && !MagGraph.Started)
            MagGraph.GraphParameters("X", "Y", "Z", "Time in secs", "Magnetic field strength  in uT", 100, windowsize, RMSLength);

        if (OrientGraph.Init && !OrientGraph.Started)
            OrientGraph.GraphParameters("X", "Y", "Z", "Time in secs", "Orientation in degree", 10, windowsize, RMSLength);

        if (PressureGraph.Init && !PressureGraph.Started)
            PressureGraph.GraphParameters("Ambinet Pressure", "Time in second", "Pressure in bar", 100000, windowsize, RMSLength);

        if (InternalTempGraph.Init && !InternalTempGraph.Started)
            InternalTempGraph.GraphParameters("Ambient Temperature", "Time in second", "Temperature in degree celsisu", 10, windowsize, RMSLength);

        if (VoltageGraph.Init && !VoltageGraph.Started)
            VoltageGraph.GraphParameters("Bus Voltage", "Shunt Voltage", "Time in secs", "Voltage in  volts", 100, windowsize, RMSLength);

        if (ExternalTempGraph.Init && !ExternalTempGraph.Started)
            ExternalTempGraph.GraphParameters(" External Probe Temperature", " Time in secs", " Temperature in celsius ", 10, windowsize, RMSLength);

        if (CurrentGraph.Init && !CurrentGraph.Started)
            CurrentGraph.GraphParameters("Current", "Time in secs", "Current in amps", 100000, windowsize, RMSLength);

        if ((Custom1Init || Custom2Init || Custom3Init) && !CustomGraph.Started && !CustomGraph.Init)
        {
            AccGraph.ConversionFactor = 100;
            GyroGraph.ConversionFactor = 100;
            MagGraph.ConversionFactor = 100;
            OrientGraph.ConversionFactor = 10;
            VoltageGraph.ConversionFactor = 100;
            PressureGraph.ConversionFactor = 100000;
            InternalTempGraph.ConversionFactor = 10;
            ExternalTempGraph.ConversionFactor = 10;
            CurrentGraph.ConversionFactor = 100000;

            if (Custom1Init)
            {
                if (YAxisCustom1 != 0 && YAxisCustom2 == 0 && YAxisCustom3 == 0)
                {
                    CustomGraph.GraphParameters(YAxisCustomLabel1, "Time in secs", "", 1, windowsize, RMSLength);
                    error = false;
                    CustomGraph.Init = true;
                }
                else
                {
                    CustomMsgBox->setIcon(QMessageBox::Critical);
                    CustomMsgBox->setText("Invalid paramters selection");
                    CustomMsgBox->setDetailedText("");
                    CustomMsgBox->exec();
                    error = true;
                }
            }
            if (Custom2Init)
            {
                if (YAxisCustom1 != 0 && YAxisCustom2 != 0 && YAxisCustom3 == 0)
                {
                    CustomGraph.GraphParameters(YAxisCustomLabel1, YAxisCustomLabel2, "Time in secs", "", 1, windowsize, RMSLength);
                    if (YAxisCustom1 == YAxisCustom2)
                    {
                        CustomMsgBox->setIcon(QMessageBox::NoIcon);
                        CustomMsgBox->setText("The selected Parameters are same");
                        CustomMsgBox->setDetailedText("");
                        CustomMsgBox->exec();
                    }
                    error = false;
                    CustomGraph.Init = true;
                }
                else
                {
                    // CustomMsgBox->setIcon( QMessageBox::Critical );
                    // CustomMsgBox->setText("Invalid paramters selection");
                    // CustomMsgBox->setDetailedText("");
                    // CustomMsgBox->exec();
                    error = true;
                }
            }
            if (Custom3Init)
            {
                if (YAxisCustom1 != 0 && YAxisCustom2 != 0 && YAxisCustom3 != 0)
                {
                    CustomGraph.GraphParameters(YAxisCustomLabel1, YAxisCustomLabel2, YAxisCustomLabel3, "Time in secs", "", 1, windowsize, RMSLength);
                    error = false;
                    CustomGraph.Init = true;
                    if (YAxisCustom1 == YAxisCustom2)
                    {
                        CustomMsgBox->setIcon(QMessageBox::NoIcon);
                        CustomMsgBox->setText("The selected Parameters are same");
                        CustomMsgBox->setDetailedText("");
                        CustomMsgBox->exec();
                    }
                }
                else
                {
                    // CustomMsgBox->setIcon( QMessageBox::Critical );
                    // CustomMsgBox->setText("Invalid paramters selection");
                    // CustomMsgBox->setDetailedText("");
                    // CustomMsgBox->exec();
                    error = true;
                }
            }
        }
        if (!binded && !error)
        {
            socket->bind(PortNo);
            connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
            binded = true;
        }
        if (!error)
        {
            CustomMsgBox->setIcon(QMessageBox::NoIcon);
            CustomMsgBox->setText("Started Plotting!");
            CustomMsgBox->setDetailedText("");
            CustomMsgBox->show();
        }
        if (error)
        {
            CustomMsgBox->setIcon(QMessageBox::NoIcon);
            CustomMsgBox->setText("Invalid Custom Plot option is selected");
            CustomMsgBox->setDetailedText("");
            CustomMsgBox->show();
        }
    }
    if (windowsize <= 0 || RMSLength <= 0)
    {
        CustomMsgBox->setIcon(QMessageBox::Critical);
        CustomMsgBox->setText("Window size and/or RMS length   has not been entered correctly!!");
        CustomMsgBox->setDetailedText("Enter a valid value for Window size and RMS Length ");
        CustomMsgBox->show();
    }
    if (PortNo <= 0)
    {
        CustomMsgBox->setIcon(QMessageBox::Critical);
        CustomMsgBox->setText("Invalid Port No");
        CustomMsgBox->setDetailedText("");
        CustomMsgBox->show();
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
    YAxisCustomLabel1 = arg1;
}
void MainWindow::on_CustomY2_textActivated(const QString &arg1)
{
    YAxisCustomLabel2 = arg1;
}
void MainWindow::on_CustomY3_textActivated(const QString &arg1)
{
    YAxisCustomLabel3 = arg1;
}

/*
 getting user input of how many different sensor values
 have to be plotted
*/
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0)
        error = false;

    if (!CustomGraph.Init)
    {
        if (index == 1)
            Custom1Init = 1;
        else
            Custom1Init = 0;

        if (index == 2)
            Custom2Init = 1;
        else
            Custom2Init = 0;

        if (index == 3)
            Custom3Init = 1;
        else
            Custom3Init = 0;
    }
    if (CustomGraph.Init == true)
    {
        CustomMsgBox->setIcon(QMessageBox::Warning);
        CustomMsgBox->setText("An instance of custom plot is already running");
        CustomMsgBox->setDetailedText("Another graph cannot be added plot");
        CustomMsgBox->show();
        error = true;
    }
}
