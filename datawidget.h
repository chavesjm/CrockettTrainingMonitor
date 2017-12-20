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

    void setAngles(double yaw, double m_pitch, double roll);
    void setInitialAngles(double yaw, double m_pitch, double roll);
    void setTurnAngles(double yaw, double m_pitch, double roll);
    void setSpeedAngles(double yawSpeed, double pitchSpeed, double rollSpeed);
    void setFrequency(double frequency);
    void setBeta(double beta);
    void setRange(double range);

private slots:
    void betaChanged(int value);

signals:

    void betaValueChanged(int value);

private:
    Ui::DataWidget *ui;

    double m_beta;
    double m_frequency;
    double m_range;

    double m_yaw, m_yaw_initial, m_yaw_turn, m_yaw_speed;
    double m_pitch, m_pitch_initial, m_pitch_turn, m_pitch_speed;
    double m_roll, m_roll_initial, m_roll_turn, m_roll_speed;
};

#endif // DATAWIDGET_H
