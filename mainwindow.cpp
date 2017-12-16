#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QMessageBox>

#include <qcustomplot/qcustomplot.h>

#define SERIAL_PORT "/dev/rfcomm0"
#define PORT_VELOCIDAD QSerialPort::Baud38400

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_yaw = 0;
    m_pitch = 0;
    m_roll = 0;

    m_yawTurn = 0;
    m_range = 0;

    this->setWindowTitle("Crockett Training");

    // Create the openGL display for the map
    Object_GL = new ObjectOpenGL(ui->OpenGLFrame);
    Object_GL->setObjectName(QString::fromUtf8("ObjectOpenGL"));
    int width = this->width() - (ui->DataFrame->width()+ui->StatusGraph->width());
    Object_GL->setGeometry(QRect(0, 0, width, height()));

    // Create the menubar
    QMenu *FileMenu = menuBar()->addMenu("&File");
    FileMenu->addSeparator();
    FileMenu->addAction("Quit", qApp, SLOT (quit()), QKeySequence(tr("Ctrl+Q")));

    // Add menu items
    QMenu *ViewMenu = menuBar()->addMenu("&View");
    ViewMenu->addAction("Front view",       Object_GL, SLOT (FrontView()),  QKeySequence(tr("Ctrl+f")));
    ViewMenu->addAction("Rear view",        Object_GL, SLOT (RearView()),   QKeySequence(tr("Ctrl+e")));
    ViewMenu->addAction("Left view",        Object_GL, SLOT (LeftView()),   QKeySequence(tr("Ctrl+l")));
    ViewMenu->addAction("Right view",       Object_GL, SLOT (RightView()),  QKeySequence(tr("Ctrl+r")));
    ViewMenu->addAction("Top view",         Object_GL, SLOT (TopView()),    QKeySequence(tr("Ctrl+t")));
    ViewMenu->addAction("Bottom view",      Object_GL, SLOT (BottomView()), QKeySequence(tr("Ctrl+b")));
    FileMenu->addSeparator();
    ViewMenu->addAction("Isometric",        Object_GL, SLOT (IsometricView()), QKeySequence(tr("Ctrl+i")));

    initGraph();

    // Timer (used for repainting the GL Window every 50 ms)
    QTimer *timerDisplay = new QTimer();
    timerDisplay->connect(timerDisplay, SIGNAL(timeout()),this, SLOT(onTimer_UpdateDisplay()));
    timerDisplay->start(75);

    //DataWidget
    m_dataWidget.reset(new DataWidget());
    connect(m_dataWidget.data(),SIGNAL(betaValueChanged(int)),this,SLOT(betaValueChanged(int)));
    ui->DataFrame->setLayout(m_dataWidget->layout());

    openConnection();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openConnection()
{
    m_dataConnector.reset(new DataConnector(SERIAL_PORT));

    connect(m_dataConnector.data(),SIGNAL(dataReaded(QByteArray)),this,SLOT(dataReceived(QByteArray)));

    if(!m_dataConnector->connection())
        QMessageBox::critical(this,"Devide Error", "Error connecting " + QString(SERIAL_PORT), QMessageBox::Ok);
}

void MainWindow::closeConnection()
{
    m_dataConnector->closeConnection();
}

// Timer event : repain the Open Gl window
void MainWindow::onTimer_UpdateDisplay()
{
    Object_GL->updateGL();

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->graph->graph(0)->addData(key, m_yaw);
      ui->graph->graph(1)->addData(key, m_pitch);
      ui->graph->graph(2)->addData(key, m_roll);
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);

      ui->StatusGraph->graph(0)->addData(key, -m_range);
      ui->StatusGraph->graph(1)->addData(key, m_yawTurn);
      ui->StatusGraph->graph(2)->addData(key, m_range);
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graph->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->graph->replot();
    ui->StatusGraph->xAxis->setRange(key,8,Qt::AlignRight);
    ui->StatusGraph->replot();
}

void MainWindow::dataReceived(QByteArray data)
{
    double yaw=0.0,pitch=0.0,roll=0.0;
    double yaw_initial=0.0,pitch_initial=0.0,roll_initial=0.0;
    double yaw_turn=0.0,pitch_turn=0.0,roll_turn=0.0;
    double beta=0.0,frequency=0.0;
    double range = 0;

    QString values(data);

    values = values.remove("\r");
    values = values.remove("\n");

    QStringList value = values.split(",");

    if(value.size() == 12)
    {

        frequency = value.at(0).toDouble();
        beta = value.at(1).toDouble();

        yaw = value.at(2).toDouble();
        pitch = value.at(3).toDouble();
        roll = value.at(4).toDouble();

        yaw_initial = value.at(5).toDouble();
        pitch_initial = value.at(6).toDouble();
        roll_initial = value.at(7).toDouble();

        yaw_turn = value.at(8).toDouble();
        pitch_turn = value.at(9).toDouble();
        roll_turn = value.at(10).toDouble();

        range = value.at(11).toDouble();

        m_yaw = yaw;
        m_pitch = pitch;
        m_roll = roll;

        m_yawTurn = yaw_turn;

        m_range = range;

        Object_GL->setAngles(-pitch,-roll,yaw);

        m_dataWidget->setAngles(yaw,pitch,roll);
        m_dataWidget->setInitialAngles(yaw_initial,pitch_initial, roll_initial);
        m_dataWidget->setTurnAngles(yaw_turn, pitch_turn, roll_turn);

        m_dataWidget->setBeta(beta);
        m_dataWidget->setFrequency(frequency);
        m_dataWidget->setRange(range);
    }
}

void MainWindow::betaValueChanged(int value)
{
    m_dataConnector->writeData(QString::number(value));
}

void MainWindow::initGraph()
{
    ui->graph->addGraph(); // blue line
    ui->graph->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->graph->addGraph(); // red line
    ui->graph->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->graph->addGraph(); // red line
    ui->graph->graph(2)->setPen(QPen(QColor(102, 204, 0)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->graph->xAxis->setTicker(timeTicker);
    ui->graph->axisRect()->setupFullAxesBox();
    ui->graph->yAxis->setRange(-180,180);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->graph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graph->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->graph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graph->yAxis2, SLOT(setRange(QCPRange)));

    ui->StatusGraph->addGraph();
    ui->StatusGraph->graph(0)->setPen(QPen(QColor(255, 110, 40)));
    ui->StatusGraph->addGraph();
    ui->StatusGraph->graph(1)->setPen(QPen(QColor(40, 110, 255)));
    ui->StatusGraph->addGraph();
    ui->StatusGraph->graph(2)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker2(new QCPAxisTickerTime);
    timeTicker2->setTimeFormat("%h:%m:%s");
    ui->StatusGraph->xAxis->setTicker(timeTicker);
    ui->StatusGraph->axisRect()->setupFullAxesBox();
    ui->StatusGraph->yAxis->setRange(-10, 10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->StatusGraph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->StatusGraph->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->StatusGraph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->StatusGraph->yAxis2, SLOT(setRange(QCPRange)));

}

