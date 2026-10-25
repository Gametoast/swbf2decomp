#pragma once

#include "PblVector.h"
#include "PblMath.h"

class PblMatrix;
class PblVector3;

class PblQuaternion
{
public:

    // FUNCTION: SWBF2 0x0043ce60
    PblQuaternion() : _fX(0.0f), _fY(0.0f), _fZ(0.0f), _fS(1.0f) {}
    PblQuaternion(float fS, float fX, float fY, float fZ)
    {
        _fS = fS;
        _fX = fX;
        _fY = fY;
        _fZ = fZ;
    }
    PblQuaternion(const PblVector3& vAxis, float fAngle)
    {
        float fSin, fCos;
        PblMath::SinCos(&fSin, &fCos, fAngle * 0.5f);
        _fX = vAxis.x * fSin;
        _fY = vAxis.y * fSin;
        _fZ = vAxis.z * fSin;
        _fS = fCos;
    }

    PblQuaternion(const PblMatrix& rMatrix);

    float GetS() const { return _fS; }
    float GetX() const { return _fX; }
    float GetY() const { return _fY; }
    float GetZ() const { return _fZ; }

    void SetS(float value) { _fS = value; }
    void SetX(float value) { _fX = value; }
    void SetY(float value) { _fY = value; }
    void SetZ(float value) { _fZ = value; }

    PblQuaternion Inverse() const
    {
        PblQuaternion q;
        q._fX = -_fX;
        q._fY = -_fY;
        q._fZ = -_fZ;
        q._fS = _fS;
        return q;
    }

    PblVector3 RotateVector(const PblVector3& v) const;

    float _fX;
    float _fY;
    float _fZ;
    float _fS;
};

PblQuaternion Interpolate(const PblQuaternion& q0, const PblQuaternion& q1, float fT);

PblQuaternion operator*(const PblQuaternion& q0, const PblQuaternion& q1);

 
