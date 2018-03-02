#include "leap_event_listener.h"

void LeapEventListener::onConnect(const Leap::Controller&)
{
    this->is_connected = true;
    emit LeapConnected(true);
}

void LeapEventListener::onDisconnect(const Leap::Controller&)
{
    this->is_connected = false;
    emit LeapConnected(false);
}

void LeapEventListener::onFrame(const Leap::Controller &controller)
{
    const Leap::Frame frame = controller.frame();
    if (frame.hands().count() > 0 && is_leap_enabled)
    {
        Leap::Hand hand = frame.hands().rightmost();
        if (hand.isValid())
        {
            Leap::Vector pos = hand.palmPosition();
            qreal pitch = hand.direction().pitch();
            qreal yaw = hand.direction().yaw();
            qreal roll = hand.palmNormal().roll();

            qreal cos_roll = qAcos(roll);
            qreal cos_pitch = qAcos(pitch);
            qreal cos_yaw = qAcos(yaw);
            qreal sin_roll = qAsin(roll);
            qreal sin_pitch = qAsin(pitch);
            qreal sin_yaw = qAsin(yaw);

            QMatrix4x4 transform_matrix(cos_yaw * cos_pitch, cos_pitch * sin_yaw, -sin_pitch, pos.x,
                                        cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll, cos_yaw * cos_roll + sin_roll * sin_yaw * sin_pitch, cos_pitch * sin_roll, -pos.z,
                                        cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll, -cos_yaw * sin_roll + cos_roll * sin_yaw * sin_pitch, cos_pitch * cos_roll, pos.y + HOME_POSITION_HEIGHT,
                                        0, 0, 0, 1);

//            QMatrix4x4 transform_matrix(cos_yaw * cos_pitch, cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll, cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll, 0,
//                                        sin_yaw * cos_pitch, sin_yaw * sin_pitch * sin_roll + cos_yaw * cos_roll, sin_yaw * sin_pitch * cos_roll - cos_yaw * sin_roll, 0,
//                                        -sin_pitch, cos_pitch * sin_roll, cos_pitch * cos_roll, 0,
//                                        0, 0, 0, 1);

            QVector<int> actuator_lengths(StewartPlatform::NUM_ACTUATORS, 0);
            for (int i = 0; i < StewartPlatform::NUM_ACTUATORS; ++i)
            {
                QVector4D eff_pos = transform_matrix * END_EFF_POS[i]; // + QVector4D(pos.x, -pos.z, pos.y + HOME_POSITION_HEIGHT, 0.0f);
                actuator_lengths[i] = static_cast<int>((eff_pos.toVector3D() - BASE_POS[i]).length() - MIN_ACTUATOR_LEN);
            }
            emit LeapFrameUpdate(actuator_lengths);
        }
    }
}
