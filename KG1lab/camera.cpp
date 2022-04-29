#include <GL/freeglut.h>

#include "camera.h"

const static float StepScale = 0.1f;

Camera::Camera()
{
    m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_target = glm::vec3(0.0f, 0.0f, 1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}


Camera::Camera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
    m_pos = Pos;
    m_target = Target;
    const float Length = sqrtf(m_target.x * m_target.x + m_target.y * m_target.y + m_target.z * m_target.z);
    m_target.x /= Length;
    m_target.y /= Length;
    m_target.z /= Length;
 //   m_target.Normalize();
    m_up = Up;
    const float Length1 = sqrtf(m_up.x * m_up.x + m_up.y * m_up.y + m_up.z * m_up.z);
    m_up.x /= Length1;
    m_up.y /= Length1;
    m_up.z /= Length1;
 //   m_up.Normalize();
}


bool Camera::OnKeyboard(int Key)
{
    bool Ret = false;

    switch (Key) {

    case GLUT_KEY_UP:
    {
        m_pos += (m_target * StepScale);
        Ret = true;
    }
    break;

    case GLUT_KEY_DOWN:
    {
        m_pos -= (m_target * StepScale);
        Ret = true;
    }
    break;

    case GLUT_KEY_LEFT:
    {
        glm::vec3 Left;
        Left.x = m_target.y * m_up.z - m_target.z * m_up.y;
        Left.y = m_target.z * m_up.x - m_target.x * m_up.z;
        Left.z = m_target.x * m_up.y - m_target.y * m_up.x;
   //     Left.Normalize();
        const float Length = sqrtf(Left.x * Left.x + Left.y * Left.y + Left.z * Left.z);
        Left.x /= Length;
        Left.y /= Length;
        Left.z /= Length;

        Left *= StepScale;
        m_pos += Left;
        Ret = true;
    }
    break;

    case GLUT_KEY_RIGHT:
    {
        glm::vec3 Right;
        Right.x = m_up.y * m_target.z - m_up.z * m_target.y;
        Right.y = m_up.z * m_target.x - m_up.x * m_target.z;
        Right.z = m_up.x * m_target.y - m_up.y * m_target.x;
    //    Right.Normalize();
        const float Length = sqrtf(Right.x * Right.x + Right.y * Right.y + Right.z * Right.z);
        Right.x /= Length;
        Right.y /= Length;
        Right.z /= Length;

        Right *= StepScale;
        m_pos += Right;
        Ret = true;
    }
    break;
    }

    return Ret;
}
