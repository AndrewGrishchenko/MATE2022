#include "RegulatorsWidget.hpp"
#include "RovSingleton.hpp"

#include <QVBoxLayout>
#include <bitset>
#include <QDateTime>

RegulatorsWidget::RegulatorsWidget(QWidget* parent)
    : QWidget(parent)
    , m_depthRegulator(new QCheckBox("Глубина", this))
    , m_yawRegulator(new QCheckBox("Курс", this))
    , m_rollRegulator(new QCheckBox("Roll", this))
    , m_pitchRegulator(new QCheckBox("Pitch", this))
{
    createLayout();
    createConnections();
}

void RegulatorsWidget::createConnections()
{
    QObject::connect(m_depthRegulator.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        std::bitset<8> regulator;
        //regulator[0] = m_depthRegulator.data()->isChecked();
        //RovSingleton::instance()->control().regulators = static_cast<quint8>(regulator.to_ulong());
        keepingDepth = RovSingleton::instance()->telimetry().depth;
        is_depth = m_depthRegulator.data()->isChecked();
    });

    QObject::connect(m_yawRegulator.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        std::bitset<8> regulator;
        //regulator[0] = m_depthRegulator.data()->isChecked();
        //RovSingleton::instance()->control().regulators = static_cast<quint8>(regulator.to_ulong());
        keepingYaw = RovSingleton::instance()->telimetry().yaw;
        is_yaw = m_yawRegulator.data()->isChecked();
    });

    QObject::connect(m_rollRegulator.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        std::bitset<8> regulator;
        //regulator[0] = m_depthRegulator.data()->isChecked();
        //RovSingleton::instance()->control().regulators = static_cast<quint8>(regulator.to_ulong());
        keepingRoll = RovSingleton::instance()->telimetry().roll;
        is_roll = m_rollRegulator.data()->isChecked();
    });

    QObject::connect(m_pitchRegulator.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        std::bitset<8> regulator;
        //regulator[0] = m_depthRegulator.data()->isChecked();
        //RovSingleton::instance()->control().regulators = static_cast<quint8>(regulator.to_ulong());
        keepingPitch = RovSingleton::instance()->telimetry().pitch;
        is_pitch = m_pitchRegulator.data()->isChecked();
    });

    m_depthTimer = new QTimer(this);
    m_depthTimer->setInterval(50);
    m_depthTimer->start();
    connect(m_depthTimer, &QTimer::timeout, this, &RegulatorsWidget::depthRegulator);

    m_yawTimer = new QTimer(this);
    m_yawTimer->setInterval(50);
    m_yawTimer->start();
    connect(m_yawTimer, &QTimer::timeout, this, &RegulatorsWidget::yawRegulator);

    m_rollTimer = new QTimer(this);
    m_rollTimer->setInterval(50);
    m_rollTimer->start();
    connect(m_rollTimer, &QTimer::timeout, this, &RegulatorsWidget::rollRegulator);

    m_pitchTimer = new QTimer(this);
    m_pitchTimer->setInterval(50);
    m_pitchTimer->start();
    connect(m_pitchTimer, &QTimer::timeout, this, &RegulatorsWidget::pitchRegulator);
}

float clamp_to180(float angle)
{
    if (angle > 180.0) return angle - 360;
    if (angle < -180.0) return angle + 360;
    return angle;
}

float clamp(float v, float max_v, float min_v)
{
    if (v > max_v) return max_v;
    if (v < min_v) return min_v;
    return v;
}

void RegulatorsWidget::depthRegulator()
{
    if (is_depth)
    {
        float current_time = QDateTime::currentMSecsSinceEpoch();

        float error = RovSingleton::instance()->telimetry().depth - keepingDepth;

        float m_power = 0;

        float power_value = error * 70;
        float diff_value = 5 / (current_time - m_depth_prev_time) * (error - m_depth_prev_error);

        m_power = clamp(power_value + diff_value, 100, -100) * 0.6;

        RovSingleton::instance()->control().axisZ = static_cast<qint8>(m_power);

        m_depth_prev_time = current_time;
        m_depth_prev_error = error;
    }
}

void RegulatorsWidget::yawRegulator()
{
    if (is_yaw)
    {
        float current_time = QDateTime::currentMSecsSinceEpoch();

        float error = RovSingleton::instance()->telimetry().yaw - keepingYaw;
        error = clamp_to180(error);

        float power_0 = 0;
        float power_1 = 0;

        float power_value = error * 0.8;
        float diff_value = 0.5 / (current_time - m_yaw_prev_time) * (error - m_yaw_prev_error);

        power_0 = clamp(power_value + diff_value, 100, -100);
        power_1 = -clamp(power_value + diff_value, 100, -100);

        RovSingleton::instance()->control().thrusterPower[0] = static_cast<qint8>(power_0);
        RovSingleton::instance()->control().thrusterPower[1] = static_cast<qint8>(power_1);

        m_yaw_prev_time = current_time;
        m_yaw_prev_error = error;
    }
}

void RegulatorsWidget::rollRegulator()
{
    if (is_roll)
    {
        float current_time = QDateTime::currentMSecsSinceEpoch();

        float error = RovSingleton::instance()->telimetry().roll - keepingRoll;
        error = clamp_to180(error);

        float power_0 = 0;
        float power_1 = 0;

        float power_value = error * 0.8;
        float diff_value = 0.5 / (current_time - m_roll_prev_time) * (error - m_roll_prev_error);

        power_0 = clamp(power_value + diff_value, 100, -100);
        power_1 = -clamp(power_value + diff_value, 100, -100);

        RovSingleton::instance()->control().thrusterPower[0] = static_cast<qint8>(power_0);
        RovSingleton::instance()->control().thrusterPower[1] = static_cast<qint8>(power_1);

        m_roll_prev_time = current_time;
        m_roll_prev_error = error;
    }
}

void RegulatorsWidget::pitchRegulator()
{
    if (is_pitch)
    {
        float current_time = QDateTime::currentMSecsSinceEpoch();

        float error = RovSingleton::instance()->telimetry().pitch - keepingPitch;
        error = clamp_to180(error);

        float power_0 = 0;
        float power_1 = 0;

        float power_value = error * 0.8;
        float diff_value = 0.5 / (current_time - m_pitch_prev_time) * (error - m_pitch_prev_error);

        power_0 = clamp(power_value + diff_value, 100, -100);
        power_1 = -clamp(power_value + diff_value, 100, -100);

        RovSingleton::instance()->control().thrusterPower[0] = static_cast<qint8>(power_0);
        RovSingleton::instance()->control().thrusterPower[1] = static_cast<qint8>(power_1);

        m_pitch_prev_time = current_time;
        m_pitch_prev_error = error;
    }
}

void RegulatorsWidget::createLayout()
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_depthRegulator.data());
    layout->addWidget(m_yawRegulator.data());
    layout->addWidget(m_rollRegulator.data());
    layout->addWidget(m_pitchRegulator.data());
    setLayout(layout);
}
