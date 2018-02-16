#ifndef LEAP_EVENT_LISTENER_H
#define LEAP_EVENT_LISTENER_H

#include <include/Leap.h>
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>

#include <vector>

using namespace Leap;

class LeapEventListener : public Listener
{
public:
    bool isConnected();

private:
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onFrame(const Controller&);

    bool is_connected;
};

#endif // LEAP_EVENT_LISTENER_H
