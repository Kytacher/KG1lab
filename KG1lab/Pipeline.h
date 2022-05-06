#pragma once

#include "glm/glm.hpp"
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)
#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/mat4x4.hpp"
#include <glm/fwd.hpp>

class Pipeline
{
private:
    glm::vec3 scale;
    glm::vec3 worldPos;
    glm::vec3 rotateInfo;
    glm::mat4 transformation;
    struct {
        glm::vec3 Pos;
        glm::vec3 Target;
        glm::vec3 Up;
    } m_camera;
    struct
    {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    } m_persProj;

public:
    Pipeline()
    {
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
        worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        scale.x = ScaleX;
        scale.y = ScaleY;
        scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        worldPos.x = x;
        worldPos.y = y;
        worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        rotateInfo.x = RotateX;
        rotateInfo.y = RotateY;
        rotateInfo.z = RotateZ;
    }

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
    {
        m_persProj.FOV = FOV;
        m_persProj.Width = Width;
        m_persProj.Height = Height;
        m_persProj.zNear = zNear;
        m_persProj.zFar = zFar;
    }

    void SetCamera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }

    const glm::mat4* getTransformation()
    {
        glm::mat4 ScaleTrans, RotateTrans, TranslationTrans, PersProjTrans;
        InitScaleTransform(ScaleTrans);
        InitRotateTransform(RotateTrans);
        InitTranslationTransform(TranslationTrans);
        InitPerspectiveProj(PersProjTrans);

        transformation = PersProjTrans * TranslationTrans * RotateTrans * ScaleTrans;
        return &transformation;
    }



    void InitScaleTransform(glm::mat4& m) const
    {
        m[0][0] = scale.x; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = scale.y; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = scale.z; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    void InitRotateTransform(glm::mat4& m) const
    {
        glm::mat4 rx, ry, rz;

        const float x = ToRadian(rotateInfo.x);
        const float y = ToRadian(rotateInfo.y);
        const float z = ToRadian(rotateInfo.z);

        rx[0][0] = 1.0f; rx[0][1] = 0.0f; rx[0][2] = 0.0f; rx[0][3] = 0.0f;
        rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
        rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x); rx[2][3] = 0.0f;
        rx[3][0] = 0.0f; rx[3][1] = 0.0f; rx[3][2] = 0.0f; rx[3][3] = 1.0f;

        ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
        ry[1][0] = 0.0f; ry[1][1] = 1.0f; ry[1][2] = 0.0f; ry[1][3] = 0.0f;
        ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y); ry[2][3] = 0.0f;
        ry[3][0] = 0.0f; ry[3][1] = 0.0f; ry[3][2] = 0.0f; ry[3][3] = 1.0f;

        rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
        rz[1][0] = sinf(z); rz[1][1] = cosf(z); rz[1][2] = 0.0f; rz[1][3] = 0.0f;
        rz[2][0] = 0.0f; rz[2][1] = 0.0f; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
        rz[3][0] = 0.0f; rz[3][1] = 0.0f; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

        m = rz * ry * rx;
    }

    void InitTranslationTransform(glm::mat4& m) const
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = worldPos.x;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = worldPos.y;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = worldPos.z;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    void InitPerspectiveProj(glm::mat4& m) const
    {
        const float ar = m_persProj.Width / m_persProj.Height;
        const float zNear = m_persProj.zNear;
        const float zFar = m_persProj.zFar;
        const float zRange = zNear - zFar;
        const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0f));

        m[0][0] = 1.0f / (tanHalfFOV * ar); m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f / tanHalfFOV; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = (-zNear - zFar) / zRange; m[2][3] = 1.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 2.0f * zFar * zNear / zRange; m[3][3] = 0.0f;
    }

};


