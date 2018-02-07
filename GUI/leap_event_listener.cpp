#include "leap_event_listener.h"

bool LeapEventListener::isConnected()
{
    return this->is_connected;
}

void LeapEventListener::onConnect(const Controller &controller)
{
    this->is_connected = true;
}

void LeapEventListener::onDisconnect(const Controller &controller)
{
    this->is_connected = false;
}

void LeapEventListener::onFrame(const Controller &controller)
{

}
