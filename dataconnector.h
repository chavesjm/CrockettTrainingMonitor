#ifndef DATACONNECTOR_H
#define DATACONNECTOR_H

#include <QObject>
#include <QSerialPort>
#include <QTextStream>

class DataConnector: public QObject
{
    Q_OBJECT

public:
    DataConnector(QString serialPort);

    bool connection();
    bool isConnected();
    bool closeConnection();

    void writeData(QString data);

signals:
    void dataReaded(QByteArray data);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort m_serialPort;
    QByteArray m_byteArray;
};

#endif // DATACONNECTOR_H
