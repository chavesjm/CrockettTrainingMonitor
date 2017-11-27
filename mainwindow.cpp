#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

#define SERIAL_PORT "/dev/rfcomm0"
#define PORT_VELOCIDAD QSerialPort::Baud38400

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the window size
    this->resize(800,600);
    this->setWindowTitle("Object viewer");

    // Create a layout in the main window
    centralWidget = new QWidget(this);
    gridLayoutWidget = new QWidget(centralWidget);
    gridLayoutWidget->setGeometry(QRect(0, 0, this->width(), this->height()));
    gridLayout = new QGridLayout(gridLayoutWidget);

    // Create the openGL display for the map
    Object_GL = new ObjectOpenGL(gridLayoutWidget);
    Object_GL->setObjectName(QString::fromUtf8("ObjectOpenGL"));
    Object_GL->setGeometry(QRect(0, 0, this->width(), this->height()));

    // Insert the Open Gl display into the layout
    gridLayout->addWidget(Object_GL, 0, 0, 1, 1);
    setCentralWidget(centralWidget);

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
    QMenu *AboutMenu = menuBar()->addMenu("?");
    AboutMenu->addAction("About Convert_STL_2_Cube", this, SLOT (handleAbout()));

    // Timer (used for repainting the GL Window every 50 ms)
    QTimer *timerDisplay = new QTimer();
    timerDisplay->connect(timerDisplay, SIGNAL(timeout()),this, SLOT(onTimer_UpdateDisplay()));
    timerDisplay->start(75);

    m_standarOutput = new QTextStream(stdout);
    m_serialPort.setPortName(SERIAL_PORT);

    if(!m_serialPort.open(QSerialPort::ReadWrite))
    {
        *m_standarOutput << "Error abriendo dispositivo" << endl;
        exit(1);
    }


    //DataWidget
    m_dataWidget.reset(new DataWidget());
    connect(m_dataWidget.data(),SIGNAL(betaValueChanged(int)),this,SLOT(betaValueChanged(int)));
    m_dataWidget->show();



    m_serialPort.setBaudRate(QSerialPort::Baud38400);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    m_serialPort.clear();
    m_serialPort.flush();

    connect(&m_serialPort, &QSerialPort::readyRead,this,&MainWindow::handleReadyRead);
    connect(&m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this,&MainWindow::handleError);
    //connect(&m_timer, &QTimer::timeout, this, &MainWindow::handleTimeout);

    m_timer.start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Timer event : repain the Open Gl window
void MainWindow::onTimer_UpdateDisplay()
{
    Object_GL->updateGL();
}

void MainWindow::handleReadyRead()
{
    if(m_serialPort.canReadLine())
    {
        m_readData.append(m_serialPort.readAll());

        *m_standarOutput << m_readData << endl;

        double yaw=0.0,pitch=0.0,roll=0.0;
        double yaw_initial=0.0,pitch_initial=0.0,roll_initial=0.0;
        double yaw_turn=0.0,pitch_turn=0.0,roll_turn=0.0;
        double beta=0.0,frequency=0.0;
        int valuePont = 0;

        QString values(m_readData);

        values = values.remove("\r");
        values = values.remove("\n");

        QStringList value = values.split(",");

        if(value.size() == 12)
        {
            *m_standarOutput << value.at(0) << endl;
            *m_standarOutput << value.at(1) << endl;
            *m_standarOutput << value.at(2) << endl;
            *m_standarOutput << value.at(3) << endl;
            *m_standarOutput << value.at(4) << endl;
            *m_standarOutput << value.at(5) << endl;
            *m_standarOutput << value.at(6) << endl;
            *m_standarOutput << value.at(7) << endl;
            *m_standarOutput << value.at(8) << endl;
            *m_standarOutput << value.at(9) << endl;
            *m_standarOutput << value.at(10) << endl;
            *m_standarOutput << value.at(11) << endl;



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

            valuePont = value.at(11).toInt();

            m_readData.clear();

            Object_GL->setAngles(-pitch,-roll,yaw);

            m_dataWidget->setAngles(yaw,pitch,roll);
            m_dataWidget->setInitialAngles(yaw_initial,pitch_initial, roll_initial);
            m_dataWidget->setTurnAngles(yaw_turn, pitch_turn, roll_turn);

            m_dataWidget->setBeta(beta);
            m_dataWidget->setFrequency(frequency);
        }
        else
            *m_standarOutput << "ERROR: Number params is not valid. Size = " << value.size() << endl;

        m_readData.clear();
    }
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ReadError)
    {
       *m_standarOutput << QObject::tr("Error :") << m_serialPort.errorString() << endl;
    }
}

void MainWindow::betaValueChanged(int value)
{
    *m_standarOutput << "Beta New Value = " << value << endl;

    QString valueString = QString::number(value);

    m_serialPort.write(valueString.toStdString().c_str(),valueString.size()+1);
}
