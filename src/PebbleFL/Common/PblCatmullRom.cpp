#include "PblCatmullRom.h"

// FUNCTION: SWBF2 0x0072af70
void PblCatmullRom::CalculatePoint(float t, PblVector3& vResult) const
{
    CalculatePoint(t, mPoints[0], mPoints[1], mPoints[2], mPoints[3], vResult);
}

// FUNCTION: SWBF2 0x0072ac00
PblCatmullRom::PblCatmullRom()
{
    for (int i = 0; i < 4; ++i)
    {
        mPoints[i].SetAll(0.0f);
    }
}

// FUNCTION: SWBF2 0x0072b260
PblVector3* PblCatmullRom::GetControlPoints()
{
    return mPoints;
}

// FUNCTION: SWBF2 0x0072b270
const PblVector3* PblCatmullRom::GetControlPoints() const
{
    return mPoints;
}

// FUNCTION: SWBF2 0x0072ac60
void PblCatmullRom::AddPoint(const PblVector3& vPoint)
{
    mPoints[0] = mPoints[1];
    mPoints[1] = mPoints[2];
    mPoints[2] = mPoints[3];
    mPoints[3] = vPoint;
}

// FUNCTION: SWBF2 0x0072acb0
void PblCatmullRom::CalculateFirstDerivative(float t, PblVector3& vResult) const
{
    CalculateFirstDerivative(t, mPoints[0], mPoints[1], mPoints[2], mPoints[3], vResult);
}

// FUNCTION: SWBF2 0x0072ace0
void PblCatmullRom::CalculateFirstDerivative(float t, const PblVector3& p0, const PblVector3& p1,
                                             const PblVector3& p2, const PblVector3& p3,
                                             PblVector3& vResult)
{
    PblVector3 a, b, c;
    a = p0 * -0.5f + p1 * 1.5f + p2 * -1.5f + p3 * 0.5f;
    b = p0 + p1 * -2.5f + p2 * 2.0f + p3 * -0.5f;
    c = p0 * -0.5f + p2 * 0.5f;
    vResult = (a * 3.0f * t + b * 2.0f) * t + c;
}

// FUNCTION: SWBF2 0x0072b000
void PblCatmullRom::CalculatePoint(float t, const PblVector3& p0, const PblVector3& p1,
                                   const PblVector3& p2, const PblVector3& p3,
                                   PblVector3& vResult)
{
    PblVector3 a, b, c;
    a = p0 * -0.5f + p1 * 1.5f + p2 * -1.5f + p3 * 0.5f;
    b = p0 + p1 * -2.5f + p2 * 2.0f + p3 * -0.5f;
    c = p0 * -0.5f + p2 * 0.5f;
    vResult = ((a * t + b) * t + c) * t + p1;
}

// FUNCTION: SWBF2 0x0072b280
void PblCatmullRom::Init(const PblVector3& p0, const PblVector3& p1, const PblVector3& p2, const PblVector3& p3)
{
    mPoints[0] = p0;
    mPoints[1] = p1;
    mPoints[2] = p2;
    mPoints[3] = p3;
}
