#include "datawidget.h"
#include "ui_datawidget.h"

#include <qdebug.h>

DataWidget::DataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataWidget)
{
    ui->setupUi(this);

    ui->LCD_AX->setVisible(false);
    ui->LCD_AY->setVisible(false);
    ui->LCD_AZ->setVisible(false);
    ui->LCD_GX->setVisible(false);
    ui->LCD_GY->setVisible(false);
    ui->LCD_GZ->setVisible(false);
    ui->LCD_MX->setVisible(false);
    ui->LCD_MY->setVisible(false);
    ui->LCD_MZ->setVisible(false);
    ui->ButtonSend->setVisible(false);
    ui->SpinBoxSound->setVisible(false);
    ui->SpinBoxValue->setVisible(false);
    ui->PB_Sound->setVisible(false);
    ui->LB_AX->setVisible(false);
    ui->LB_AY->setVisible(false);
    ui->LB_AZ->setVisible(false);
    ui->LB_MX->setVisible(false);
    ui->LB_MY->setVisible(false);
    ui->LB_MZ->setVisible(false);
    ui->LB_GX->setVisible(false);
    ui->LB_GY->setVisible(false);
    ui->LB_GZ->setVisible(false);
    ui->LCD_Roll_Speed->setVisible(false);
    ui->LCD_Yaw_Speed->setVisible(false);
    ui->LCD_Pitch_Speed->setVisible(false);
    ui->LB_RS->setVisible(false);
    ui->LB_YS->setVisible(false);
    ui->LB_PS->setVisible(false);

    connect(ui->ButtonSend,SIGNAL(clicked(bool)),this,SLOT(sendValue()));
    connect(ui->PB_Sound,SIGNAL(clicked(bool)),this,SLOT(sendSound()));
}

DataWidget::~DataWidget()
{
    delete ui;
}

void DataWidget::setAcceleration(double x, double y, double z)
{
    QString xString = QString::number(x);
    QString yString = QString::number(y);
    QString zString = QString::number(z);

    ui->LCD_AX->display(xString);
    ui->LCD_AY->display(yString);
    ui->LCD_AZ->display(zString);

}

void DataWidget::setGyroscope(double x, double y, double z)
{
    QString xString = QString::number(x);
    QString yString = QString::number(y);
    QString zString = QString::number(z);

    ui->LCD_GX->display(xString);
    ui->LCD_GY->display(yString);
    ui->LCD_GZ->display(zString);

}

void DataWidget::setMagnetometer(double x, double y, double z)
{
    QString xString = QString::number(x);
    QString yString = QString::number(y);
    QString zString = QString::number(z);

    ui->LCD_MX->display(xString);
    ui->LCD_MY->display(yString);
    ui->LCD_MZ->display(zString);

}

void DataWidget::setAngles(double yaw, double pitch, double roll)
{
    m_yaw = yaw;
    m_pitch = pitch;
    m_roll = roll;

    QString yawString = QString::number(m_yaw,'f',1);
    QString pitchString = QString::number(m_pitch,'f',1);
    QString rollString = QString::number(m_roll,'f',1);

    ui->LCD_Yaw->display(yawString);
    ui->LCD_Pitch->display(pitchString);
    ui->LCD_Roll->display(rollString);

}

void DataWidget::setInitialAngles(double yaw, double pitch, double roll)
{
    m_yaw_initial = yaw;
    m_pitch_initial = pitch;
    m_roll_initial = roll;

    QString yawString = QString::number(m_yaw_initial,'f',1);
    QString pitchString = QString::number(m_pitch_initial,'f',1);
    QString rollString = QString::number(m_roll_initial,'f',1);

    ui->LCD_Yaw_Initial->display(yawString);
    ui->LCD_Pitch_Initial->display(pitchString);
    ui->LCD_Roll_Initial->display(rollString);

}

void DataWidget::setTurnAngles(double yaw, double pitch, double roll)
{
    m_yaw_turn = yaw;
    m_pitch_turn = pitch;
    m_roll_turn = roll;

    QString yawString = QString::number(m_yaw_turn,'f',1);
    QString pitchString = QString::number(m_pitch_turn,'f',1);
    QString rollString = QString::number(m_roll_turn,'f',1);

    ui->LCD_Yaw_Turn->display(yawString);
    ui->LCD_Pitch_Turn->display(pitchString);
    ui->LCD_Roll_Turn->display(rollString);

}

void DataWidget::setSpeedAngles(double yawSpeed, double pitchSpeed, double rollSpeed)
{
    m_yaw_speed = yawSpeed;
    m_pitch_speed = pitchSpeed;
    m_roll_speed = rollSpeed;

    QString yawSpeedString = QString::number(m_yaw_speed,'f',7);
    QString pitchSpeedString = QString::number(m_pitch_speed,'f',7);
    QString rollSpeedString = QString::number(m_roll_speed,'f',7);

    ui->LCD_Yaw_Speed->display(yawSpeedString);
    ui->LCD_Pitch_Speed->display(pitchSpeedString);
    ui->LCD_Roll_Speed->display(rollSpeedString);

}

void DataWidget::setBeta(double beta)
{
    m_beta = beta;

    QString betaString = QString::number(m_beta,'g');

    ui->LCD_Beta->display(betaString);

}

void DataWidget::setFrequency(double frequency)
{
    m_frequency = frequency;

    QString frequencyString = QString::number((int)m_frequency);

    ui->LCD_Frequency->display(frequencyString);
}

void DataWidget::setRange(double range)
{
    m_range = range;

    QString rangeString = QString::number(m_range);

    ui->LCD_Range->display(rangeString);
}

void DataWidget::sendValue()
{
    QString value = QString::number(ui->SpinBoxValue->value());

    emit sendValue(value);
}

void DataWidget::sendSound()
{
    QString value = "#";

    if(ui->SpinBoxSound->value() < 0)
    {
        value.append("*");
    }
    else
    {
        value.append(QString::number(ui->SpinBoxSound->value()));
    }

    emit sendValue(value);
}



