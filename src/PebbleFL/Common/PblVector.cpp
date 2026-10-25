#include <math.h>

#include "PblVector.h"
#include "PblMatrix.h"



// FUNCTION: SWBF2 0x00729f90
const PblBox PblBox::GetTransformedBox(const PblMatrix& rMatrix) const
{
    PblBox box;
    box._vMin = *(const PblVector3*)&rMatrix.m[3][0];
    box._vMax = box._vMin;
    const float* pMin = &_vMin.x;
    const float* pMax = &_vMax.x;
    float* pOutMin = &box._vMin.x;
    float* pOutMax = &box._vMax.x;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            float f = rMatrix.m[i][j];
            if (f < 0.0f)
            {
                pOutMax[j] += f * pMin[i];
                pOutMin[j] += f * pMax[i];
            }
            else
            {
                pOutMax[j] += f * pMax[i];
                pOutMin[j] += f * pMin[i];
            }
        }
    }
    return box;
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
