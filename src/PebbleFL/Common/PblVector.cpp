#include <math.h>

#include "PblVector.h"
#include "PblMatrix.h"



// FUNCTION: SWBF2 0x00729f90
const PblBox PblBox::GetTransformedBox(const PblMatrix& m) const
{
    PblVector3 vMin = m.GetTranslation();
    PblVector3 vMax = vMin;
    for (int i = 0; i < 3; ++i)
    {
        const float _vMin_i = _vMin[i];
        const float _vMax_i = _vMax[i];
        for (int j = 0; j < 3; ++j)
        {
            if (m(i, j) >= 0.0f)
            {
                vMin[j] += m(i, j) * _vMin_i;
                vMax[j] += m(i, j) * _vMax_i;
            }
            else
            {
                vMin[j] += m(i, j) * _vMax_i;
                vMax[j] += m(i, j) * _vMin_i;
            }
        }
    }
    return PblBox(vMin, vMax);
}

// GLOBAL: SWBF2 0x007e2d14
PblVector3 gVectorUnitX(1.0f, 0.0f, 0.0f);

// GLOBAL: SWBF2 0x007e2d20
PblVector3 gVectorUnitY(0.0f, 1.0f, 0.0f);

// GLOBAL: SWBF2 0x007e2d2c
PblVector3 gVectorUnitZ(0.0f, 0.0f, 1.0f);

// FUNCTION: SWBF2 0x0040ca00
float PblVector3::GetX() const
{
    return x;
}

// FUNCTION: SWBF2 0x0040ca10
float PblVector3::GetY() const
{
    return y;
}

// FUNCTION: SWBF2 0x0040ca20
float PblVector3::GetZ() const
{
    return z;
}
