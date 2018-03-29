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
            qreal x = pos.x;
            qreal y = pos.y;
            qreal z = pos.z;

            qreal pitch = hand.direction().pitch();
            qreal yaw = hand.direction().yaw();
            qreal roll = hand.palmNormal().roll();

            qreal cos_roll = qAcos(roll);
            qreal cos_pitch = qAcos(pitch);
            qreal cos_yaw = qAcos(yaw);
            qreal sin_roll = qAsin(roll);
            qreal sin_pitch = qAsin(pitch);
            qreal sin_yaw = qAsin(yaw);

            // Define the affine matrix transformation explicitly
            qreal m_11 = cos_yaw * cos_pitch;
            qreal m_12 = cos_pitch * sin_yaw;
            qreal m_13 = -sin_pitch;
            qreal m_14 = x;

            qreal m_21 = cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll;
            qreal m_22 = cos_yaw * cos_roll + sin_roll * sin_yaw * sin_pitch;
            qreal m_23 = cos_pitch * sin_roll;
            qreal m_24 = -z;

            qreal m_31 = cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll;
            qreal m_32 = -cos_yaw * sin_roll + cos_roll * sin_yaw * sin_pitch;
            qreal m_33 = cos_pitch * cos_roll;
            qreal m_34 = y + HOME_POSITION_HEIGHT;

            qreal m_41 = 0;
            qreal m_42 = 0;
            qreal m_43 = 0;
            qreal m_44 = 1;

//            QMatrix4x4 transform_matrix(cos_yaw * cos_pitch, cos_pitch * sin_yaw, -sin_pitch, pos.x,
//                                        cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll, cos_yaw * cos_roll + sin_roll * sin_yaw * sin_pitch, cos_pitch * sin_roll, -pos.z,
//                                        cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll, -cos_yaw * sin_roll + cos_roll * sin_yaw * sin_pitch, cos_pitch * cos_roll, pos.y + HOME_POSITION_HEIGHT,
//                                        0, 0, 0, 1);

//            QMatrix4x4 transform_matrix(cos_yaw * cos_pitch, cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll, cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll, 0,
//                                        sin_yaw * cos_pitch, sin_yaw * sin_pitch * sin_roll + cos_yaw * cos_roll, sin_yaw * sin_pitch * cos_roll - cos_yaw * sin_roll, 0,
//                                        -sin_pitch, cos_pitch * sin_roll, cos_pitch * cos_roll, 0,
//                                        0, 0, 0, 1);
            // Obtain actuator lengths from applying the affine transformation to the end-effector positions
            qreal x_1;
            qreal x_2;
            qreal x_3;
            qreal x_4;
            QVector4D eff_pos;
            QVector<int> actuator_lengths(StewartPlatform::NUM_ACTUATORS, 0);
            for (int i = 0; i < StewartPlatform::NUM_ACTUATORS; ++i)
            {
//                QVector4D eff_pos = transform_matrix * END_EFF_POS[i]; // + QVector4D(pos.x, -pos.z, pos.y + HOME_POSITION_HEIGHT, 0.0f);
//                QVector4D eff_pos = transform_matrix * END_EFF_POS[i] + QVector4D(pos.x, -pos.z, pos.y + HOME_POSITION_HEIGHT, 0.0f);

                x_1 = END_EFF_POS[i][0];
                x_2 = END_EFF_POS[i][1];
                x_3 = END_EFF_POS[i][2];
                x_4 = END_EFF_POS[i][3];

                eff_pos = QVector4D
                (
                    (m_11 * x_1) + (m_12 * x_2) + (m_13 * x_3) + (m_14 * x_4),
                    (m_21 * x_1) + (m_22 * x_2) + (m_23 * x_3) + (m_24 * x_4),
                    (m_31 * x_1) + (m_32 * x_2) + (m_33 * x_3) + (m_34 * x_4),
                    (m_41 * x_1) + (m_42 * x_2) + (m_43 * x_3) + (m_44 * x_4)
                );

                actuator_lengths[i] = static_cast<int>((eff_pos.toVector3D() - BASE_POS[i]).length() - MIN_ACTUATOR_LEN);
            }
            emit LeapFrameUpdate(actuator_lengths);
        }
    }
}
