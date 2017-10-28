#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>

namespace Ui {
class DataWidget;
}

class DataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataWidget(QWidget *parent = 0);
    ~DataWidget();

    void setAngles(double theta, double psi, double phi);
    void setInitialAngles(double theta, double psi, double phi);
    void setTurnAngles(double theta, double psi, double phi);
    void setFrequency(double frequency);
    void setBeta(double beta);

private slots:
    void on_DI_Beta_valueChanged(int value);

signals:

    void betaValueChanged(int value);

private:
    Ui::DataWidget *ui;

    double m_beta;
    double m_frequency;

    double m_theta, m_theta_initial, m_theta_turn;
    double m_psi, m_psi_initial, m_psi_turn;
    double m_phi, m_phi_initial, m_phi_turn;
};

#endif // DATAWIDGET_H
