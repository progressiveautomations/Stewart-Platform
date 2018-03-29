#ifndef LEAP_EVENT_LISTENER_H
#define LEAP_EVENT_LISTENER_H

#include <include/Leap.h>
#include <QtMath>
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>
#include <QVector>

#include "stewart_platform.h"


class LeapEventListener : public QObject, public Leap::Listener
{
    Q_OBJECT
public:
    bool is_leap_enabled = false;

signals:
    void LeapConnected(bool);
    void LeapFrameUpdate(QVector<int>);

private:
    virtual void onConnect(const Leap::Controller&);
    virtual void onDisconnect(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);

    bool is_connected;
    const QVector<QVector3D> BASE_POS
    {
        QVector3D(-246.34f, 86.42f, 0.0f),
        QVector3D(-198.16f, 170.38f, 0.0f),
        QVector3D(198.16f, 170.38f, 0.0f),
        QVector3D(246.34f, 86.42f, 0.0f),
        QVector3D(48.48f, -256.80f, 0.0f),
        QVector3D(-48.48f, -256.80f, 0.0f)
    };

    const QVector<QVector4D> END_EFF_POS
    {
        QVector4D(-225.6f, -73.26f, 0.0f, 1.0f),
        QVector4D(-49.35f, 232.01f, 0.0f, 1.0f),
        QVector4D(49.35f, 232.01f, 0.0f, 1.0f),
        QVector4D(225.60f, -73.26f, 0.0f, 1.0f),
        QVector4D(176.25f, -158.75f, 0.0f, 1.0f),
        QVector4D(-176.25f, -158.75f, 0.0f, 1.0f)
    };
    const qreal HOME_POSITION_HEIGHT = 319.0f;
    const qreal MIN_ACTUATOR_LEN = 335.0f;
};

#endif // LEAP_EVENT_LISTENER_H
