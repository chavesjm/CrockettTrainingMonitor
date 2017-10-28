#include "datawidget.h"
#include "ui_datawidget.h"

DataWidget::DataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataWidget)
{
    ui->setupUi(this);
}

DataWidget::~DataWidget()
{
    delete ui;
}

void DataWidget::setAngles(double theta, double psi, double phi)
{
    m_theta = theta;
    m_psi = psi;
    m_phi = phi;

    QString thetaString = QString::number((int)m_theta);
    QString psiString = QString::number((int)m_psi);
    QString phiString = QString::number((int)m_phi);

    ui->LCD_Theta->display(thetaString);
    ui->LCD_Psi->display(psiString);
    ui->LCD_Phi->display(phiString);

}

void DataWidget::setInitialAngles(double theta, double psi, double phi)
{
    m_theta_initial = theta;
    m_psi_initial = psi;
    m_phi_initial = phi;

    QString thetaString = QString::number((int)m_theta_initial);
    QString psiString = QString::number((int)m_psi_initial);
    QString phiString = QString::number((int)m_phi_initial);

    ui->LCD_Theta_Initial->display(thetaString);
    ui->LCD_Psi_Initial->display(psiString);
    ui->LCD_Phi_Initial->display(phiString);

}

void DataWidget::setTurnAngles(double theta, double psi, double phi)
{
    m_theta_turn = theta;
    m_psi_turn = psi;
    m_phi_turn = phi;

    QString thetaString = QString::number(m_theta_turn);
    QString psiString = QString::number(m_psi_turn);
    QString phiString = QString::number(m_phi_turn);

    ui->LCD_Theta_Turn->display(thetaString);
    ui->LCD_Psi_Turn->display(psiString);
    ui->LCD_Phi_Turn->display(phiString);

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

void DataWidget::on_DI_Beta_valueChanged(int value)
{
    emit betaValueChanged(value);
}

