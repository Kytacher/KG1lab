#include <GL/freeglut.h>

#include "camera.h"
#include "Pipeline.h"


const static float StepScale = 0.1f;
const static int MARGIN = 10;








struct Quaternion
{
    float x, y, z, w;

    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    Quaternion Conjugate();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const glm::vec3& v);

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

void Quaternion::Normalize()
{
    float Length = sqrtf(x * x + y * y + z * z + w * w);

    x /= Length;
    y /= Length;
    z /= Length;
    w /= Length;
}

Quaternion Quaternion::Conjugate()
{
    Quaternion ret(-x, -y, -z, w);
    return ret;
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
    const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
    const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
    const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
    const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

    Quaternion ret(x, y, z, w);

    return ret;
}

Quaternion operator*(const Quaternion& q, const glm::vec3& v)
{
    const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    const float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    const float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    const float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

    Quaternion ret(x, y, z, w);

    return ret;
}














Camera::Camera(int WindowWidth, int WindowHeight)
{
    m_windowWidth = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_target = glm::vec3(0.0f, 0.0f, 1.0f);
    const float Length = sqrtf(m_target.x * m_target.x + m_target.y * m_target.y + m_target.z * m_target.z);
    m_target.x /= Length;
    m_target.y /= Length;
    m_target.z /= Length;
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    Init();
}


Camera::Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
    m_windowWidth = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = Pos;
    m_target = Target;
    const float Length = sqrtf(m_target.x * m_target.x + m_target.y * m_target.y + m_target.z * m_target.z);
    m_target.x /= Length;
    m_target.y /= Length;
    m_target.z /= Length;

    m_up = Up;
    const float Length1 = sqrtf(m_up.x * m_up.x + m_up.y * m_up.y + m_up.z * m_up.z);
    m_up.x /= Length1;
    m_up.y /= Length1;
    m_up.z /= Length1;

    Init();
}

void Camera::Init()
{
    glm::vec3 HTarget(m_target.x, 0.0, m_target.z);
    const float Length1 = sqrtf(HTarget.x * HTarget.x + HTarget.y * HTarget.y + HTarget.z * HTarget.z);
    HTarget.x /= Length1;
    HTarget.y /= Length1;
    HTarget.z /= Length1;

    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
        }
        else
        {
            m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = ToDegree(asin(-HTarget.z));
        }
        else
        {
            m_AngleH = 90.0f + ToDegree(asin(-HTarget.z));
        }
    }

    m_AngleV = -ToDegree(asin(m_target.y));

    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge = false;
    m_OnRightEdge = false;
    m_mousePos.x = m_windowWidth / 2;
    m_mousePos.y = m_windowHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
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

void Camera::OnMouse(int x, int y)
{
    const int DeltaX = x - m_mousePos.x;
    const int DeltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_AngleH += (float)DeltaX / 20.0f;
    m_AngleV += (float)DeltaY / 20.0f;

    if (DeltaX == 0) {
        if (x <= MARGIN) {
            m_OnLeftEdge = true;
        }
        else if (x >= (m_windowWidth - MARGIN)) {
            m_OnRightEdge = true;
        }
    }
    else {
        m_OnLeftEdge = false;
        m_OnRightEdge = false;
    }

    if (DeltaY == 0) {
        if (y <= MARGIN) {
            m_OnUpperEdge = true;
        }
        else if (y >= (m_windowHeight - MARGIN)) {
            m_OnLowerEdge = true;
        }
    }
    else {
        m_OnUpperEdge = false;
        m_OnLowerEdge = false;
    }

    Update();
}

void Camera::OnRender()
{
    bool ShouldUpdate = false;

    if (m_OnLeftEdge) {
        m_AngleH -= 0.1f;
        ShouldUpdate = true;
    }
    else if (m_OnRightEdge) {
        m_AngleH += 0.1f;
        ShouldUpdate = true;
    }

    if (m_OnUpperEdge) {
        if (m_AngleV > -90.0f) {
            m_AngleV -= 0.1f;
            ShouldUpdate = true;
        }
    }
    else if (m_OnLowerEdge) {
        if (m_AngleV < 90.0f) {
            m_AngleV += 0.1f;
            ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}

void Camera::Update()
{
    const glm::vec3 Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    glm::vec3 View(1.0f, 0.0f, 0.0f);

    const float SinHalfAngle = sinf(ToRadian(m_AngleH / 2));
    const float CosHalfAngle = cosf(ToRadian(m_AngleH / 2));

    const float Rx = Vaxis.x * SinHalfAngle;
    const float Ry = Vaxis.y * SinHalfAngle;
    const float Rz = Vaxis.z * SinHalfAngle;
    const float Rw = CosHalfAngle;
    Quaternion RotationQ(Rx, Ry, Rz, Rw);

    Quaternion ConjugateQ = RotationQ.Conjugate();
    //  ConjugateQ.Normalize();
    Quaternion W = RotationQ * ConjugateQ;

    View.x = W.x;
    View.y = W.y;
    View.z = W.z;


    const float Length0 = sqrtf(View.x * View.x + View.y * View.y + View.z * View.z);
    View.x /= Length0;
    View.y /= Length0;
    View.z /= Length0;

    // Rotate the view vector by the vertical angle around the horizontal axis
    glm::vec3 Haxis;
    Haxis.x = Vaxis.y * View.z - Vaxis.z * View.y;
    Haxis.y = Vaxis.z * View.x - Vaxis.x * View.z;
    Haxis.z = Vaxis.x * View.y - Vaxis.y * View.x;
    const float Length = sqrtf(Haxis.x * Haxis.x + Haxis.y * Haxis.y + Haxis.z * Haxis.z);
    Haxis.x /= Length;
    Haxis.y /= Length;
    Haxis.z /= Length;

    const float SinHalfAngle1 = sinf(ToRadian(m_AngleV / 2));
    const float CosHalfAngle1 = cosf(ToRadian(m_AngleV / 2));

    const float Rx1 = Haxis.x * SinHalfAngle1;
    const float Ry1 = Haxis.y * SinHalfAngle1;
    const float Rz1 = Haxis.z * SinHalfAngle1;
    const float Rw1 = CosHalfAngle1;
    Quaternion RotationQ1(Rx1, Ry1, Rz1, Rw1);
    
    Quaternion ConjugateQ1 = RotationQ.Conjugate();
    //  ConjugateQ.Normalize();
    Quaternion W1 = RotationQ1 * ConjugateQ1;

    Haxis.x = W1.x;
    Haxis.y = W1.y;
    Haxis.z = W1.z;

    m_target = View;
    const float Length1 = sqrtf(m_target.x * m_target.x + m_target.y * m_target.y + m_target.z * m_target.z);
    m_target.x /= Length1;
    m_target.y /= Length1;
    m_target.z /= Length1;
    
    m_up.x = m_target.y * Haxis.z - m_target.z * Haxis.y;
    m_up.y = m_target.z * Haxis.x - m_target.x * Haxis.z;
    m_up.z = m_target.x * Haxis.y - m_target.y * Haxis.x;
    const float Length2 = sqrtf(m_up.x * m_up.x + m_up.y * m_up.y + m_up.z * m_up.z);
    m_up.x /= Length2;
    m_up.y /= Length2;
    m_up.z /= Length2;
}
