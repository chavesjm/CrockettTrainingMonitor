#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QTimer>
#include <QGridLayout>
#include <objectgl.h>
#include <datawidget.h>

#include <dataconnector.h>

#include <QUdpSocket>

#include <qcustomplot/qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onTimer_UpdateDisplay();
    void dataReceived(QByteArray data);
    void dataUDPReceived();
    void sendData(QString data);

private:
    void openConnection();
    void openUDPConnection();
    void closeConnection();
    void closeUDPConnection();
    void initGraph();

    Ui::MainWindow *ui;

    QTimer  m_timer, m_timerRead;

    float m_yaw, m_pitch, m_roll;
    float m_yawTurn;
    float m_range;

    QCPCurve *m_newCurve;

    // Layout of the window
    QGridLayout             *gridLayout;
    QWidget                 *gridLayoutWidget;

    // Central widget (where the openGL window is drawn)
    QWidget                 *centralWidget;

    // OpenGL object
    ObjectOpenGL            *Object_GL;

    //DataWidget
    QScopedPointer<DataWidget> m_dataWidget;

    QScopedPointer<DataConnector> m_dataConnector;

    QScopedPointer<QUdpSocket> m_udpSocket;
};

#endif // MAINWINDOW_H
