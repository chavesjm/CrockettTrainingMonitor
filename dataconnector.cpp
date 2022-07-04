#include "dataconnector.h"

#include <QDebug>
#include <stdlib.h>
#include <iostream>

DataConnector::DataConnector(QString serialPort)
{
    m_serialPort.setPortName(serialPort);

    QObject::connect(&m_serialPort, &QSerialPort::readyRead,this,&DataConnector::handleReadyRead);
    QObject::connect(&m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this,&DataConnector::handleError);
}

bool DataConnector::connection()
{
    m_serialPort.setBaudRate(QSerialPort::Baud115200);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    m_serialPort.open(QSerialPort::ReadOnly);

    std::cout << m_serialPort.error() << std::endl;

    m_serialPort.clear();
    m_serialPort.flush();

    return isConnected();
}

bool DataConnector::closeConnection()
{
    m_serialPort.close();
}

bool DataConnector::isConnected()
{
    return m_serialPort.isOpen();
}

void DataConnector::handleReadyRead()
{
    if(m_serialPort.canReadLine())
    {
        QByteArray byteArray = m_serialPort.readAll();

        std::cout << QString(byteArray).toStdString() << std::endl;

        emit dataReaded(byteArray);
    }
}

void DataConnector::handleError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ReadError)
    {
       qDebug() << QObject::tr("Error :") << m_serialPort.errorString();
    }
}

void DataConnector::writeData(QString data)
{
    data.insert(0,"<");
    data.append(">");

    qDebug() << "DataConnector::writeData() = " << data;

    m_serialPort.write(data.toStdString().c_str(),data.size()+1);
}
