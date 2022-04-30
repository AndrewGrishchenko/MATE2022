#pragma once

#include <QCheckBox>
#include <QWidget>

class RegulatorsWidget : public QWidget {
    Q_OBJECT
public:
    explicit RegulatorsWidget(QWidget* parent = nullptr);

signals:

public slots:
private:
    void createConnections();
    void createLayout();

    QScopedPointer<QCheckBox> m_depthRegulator;
    bool is_depth = false;
    QTimer* m_depthTimer;
    void depthRegulator();
    float keepingDepth = 0;
    float m_depth_kp = 0.6;
    float m_depth_kd = 5;
    float m_depth_prev_time;
    float m_depth_prev_error;

    QScopedPointer<QCheckBox> m_yawRegulator;
    bool is_yaw = false;
    QTimer* m_yawTimer;
    void yawRegulator();
    float keepingYaw = 0;
    float m_yaw_kp = 0.6;
    float m_yaw_kd = 5;
    float m_yaw_prev_time;
    float m_yaw_prev_error;

    QScopedPointer<QCheckBox> m_rollRegulator;
    bool is_roll = false;
    QTimer* m_rollTimer;
    void rollRegulator();
    float keepingRoll = 0;
    float m_roll_kp = 0.6;
    float m_roll_kd = 5;
    float m_roll_prev_time;
    float m_roll_prev_error;

    QScopedPointer<QCheckBox> m_pitchRegulator;
    bool is_pitch = false;
    QTimer* m_pitchTimer;
    void pitchRegulator();
    float keepingPitch = 0;
    float m_pitch_kp = 0.6;
    float m_pitch_kd = 5;
    float m_pitch_prev_time;
    float m_pitch_prev_error;
};
