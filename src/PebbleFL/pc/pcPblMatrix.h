#pragma once

#include <d3dx9math.h>

inline void PblMatrix::SetTranslation(const PblVector3& vTranslation)
{
    *reinterpret_cast<PblVector3*>(m[3]) = vTranslation;
    m[3][3] = 1.0f;
}

inline PblVector3& PblMatrix::operator[](int iRow)
{
    return *reinterpret_cast<PblVector3*>(m[iRow]);
}

inline const PblVector3& PblMatrix::operator[](int iRow) const
{
    return *reinterpret_cast<const PblVector3*>(m[iRow]);
}

inline void PblMatrix::Transpose3x3()
{
    float fSwap = m[1][0];
    m[1][0] = m[0][1];
    m[0][1] = fSwap;
    fSwap = m[2][0];
    m[2][0] = m[0][2];
    m[0][2] = fSwap;
    fSwap = m[2][1];
    m[2][1] = m[1][2];
    m[1][2] = fSwap;
}

// FUNCTION: SWBF2 0x0043a8c0
inline PblMatrix::PblMatrix(const PblQuaternion& rotation, const PblVector3& translation)
{
    D3DXQUATERNION normalized;
    D3DXMatrixRotationQuaternion((D3DXMATRIX*)this,
        D3DXQuaternionNormalize(&normalized, (const D3DXQUATERNION*)&rotation));
    m[3][0] = translation.x;
    m[3][1] = translation.y;
    m[3][2] = translation.z;
    m[3][3] = 1.0f;
}

inline const PblMatrix operator*(const PblMatrix& a, const PblMatrix& b)
{
    PblMatrix result;
    D3DXMatrixMultiply((D3DXMATRIX*)&result, (const D3DXMATRIX*)&a, (const D3DXMATRIX*)&b);
    return result;
}
