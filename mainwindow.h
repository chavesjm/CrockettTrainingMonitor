#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QTimer>
#include <QGridLayout>
#include <objectgl.h>
#include <datawidget.h>

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
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void betaValueChanged(int value);

private:
    Ui::MainWindow *ui;
    QSerialPort m_serialPort;
    QByteArray m_readData;
    QTextStream *m_standarOutput;
    QTimer  m_timer, m_timerRead;

    // Layout of the window
    QGridLayout             *gridLayout;
    QWidget                 *gridLayoutWidget;

    // Central widget (where the openGL window is drawn)
    QWidget                 *centralWidget;

    // OpenGL object
    ObjectOpenGL            *Object_GL;

    //DataWidget
    QScopedPointer<DataWidget> m_dataWidget;
};

#endif // MAINWINDOW_H
