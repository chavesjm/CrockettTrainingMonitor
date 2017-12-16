#include "datawidget.h"
#include "ui_datawidget.h"

DataWidget::DataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataWidget)
{
    ui->setupUi(this);

    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(betaChanged()));
}

DataWidget::~DataWidget()
{
    delete ui;
}

void DataWidget::setAngles(double yaw, double pitch, double roll)
{
    m_yaw = yaw;
    m_pitch = pitch;
    m_roll = roll;

    QString yawString = QString::number((int)m_yaw);
    QString pitchString = QString::number((int)m_pitch);
    QString rollString = QString::number((int)m_roll);

    ui->LCD_Yaw->display(yawString);
    ui->LCD_Pitch->display(pitchString);
    ui->LCD_Roll->display(rollString);

}

void DataWidget::setInitialAngles(double yaw, double pitch, double roll)
{
    m_yaw_initial = yaw;
    m_pitch_initial = pitch;
    m_roll_initial = roll;

    QString yawString = QString::number((int)m_yaw_initial);
    QString pitchString = QString::number((int)m_pitch_initial);
    QString rollString = QString::number((int)m_roll_initial);

    ui->LCD_Yaw_Initial->display(yawString);
    ui->LCD_Pitch_Initial->display(pitchString);
    ui->LCD_Roll_Initial->display(rollString);

}

void DataWidget::setTurnAngles(double yaw, double pitch, double roll)
{
    m_yaw_turn = yaw;
    m_pitch_turn = pitch;
    m_roll_turn = roll;

    QString yawString = QString::number(m_yaw_turn);
    QString pitchString = QString::number(m_pitch_turn);
    QString rollString = QString::number(m_roll_turn);

    ui->LCD_Yaw_Turn->display(yawString);
    ui->LCD_Pitch_Turn->display(pitchString);
    ui->LCD_Roll_Turn->display(rollString);

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

void DataWidget::betaChanged(int value)
{
    emit betaValueChanged(value);
}

