#include "dataconnector.h"

#include <QDebug>
#include <stdlib.h>
#include <iostream>
#include <QDateTime>

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
    m_serialPort.setFlowControl(QSerialPort::HardwareControl);

    m_serialPort.open(QSerialPort::ReadWrite);

    std::cout << m_serialPort.error() << std::endl;

    m_serialPort.clear();
    m_serialPort.flush();

    return isConnected();
}

bool DataConnector::closeConnection()
{
    m_serialPort.close();

    return true;
}

bool DataConnector::isConnected()
{
    return m_serialPort.isOpen();
}

void DataConnector::handleReadyRead()
{
    QByteArray byteArray = m_serialPort.readAll();

    while(byteArray.contains("\n")){
        int pos = byteArray.indexOf("\n");
        m_byteArray.append(byteArray.left(pos + 1));
        std::cout << (QString::number(QDateTime::currentMSecsSinceEpoch()) + QString(";") + QString(m_byteArray)).toStdString() << std::endl;
        emit dataReaded(m_byteArray);
        m_byteArray.clear();
        byteArray.remove(0,pos+1);
    }

    m_byteArray.append(byteArray);
}

void DataConnector::handleError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ReadError)
    {
       qDebug() << QObject::tr("Error :") << m_serialPort.errorString();
    }
}

void DataConnector::writeChar(char value){

    m_serialPort.write(&value,sizeof(char));
}

void DataConnector::writeData(QString data)
{
    data.insert(0,"<");
    data.append(">");

    qDebug() << "DataConnector::writeData() = " << data;

    m_serialPort.write(data.toStdString().c_str(),data.size()+1);
}
