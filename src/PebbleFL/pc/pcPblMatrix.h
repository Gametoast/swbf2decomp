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

// FUNCTION: SWBF2 0x00411dd0
inline const PblMatrix PblMatrix::GetInverseFast() const
{
    PblMatrix out;

    out = *this;

    out.Transpose3x3();

    PblVector3 vRotated;
    D3DXVec3TransformNormal(reinterpret_cast<D3DXVECTOR3*>(&vRotated),
                            reinterpret_cast<const D3DXVECTOR3*>(&out.GetTranslation()),
                            reinterpret_cast<const D3DXMATRIX*>(&out));

    out.SetTranslation(PblVector3(-vRotated.x, -vRotated.y, -vRotated.z));

    return out;
}

// FUNCTION: SWBF2 0x00458960
inline void PblMatrix::Reset()
{
    D3DXMatrixIdentity(reinterpret_cast<D3DXMATRIX*>(this));
}

inline void PblMatrix::SetXAxis(float fX, float fY, float fZ, float fW)
{
    m[0][0] = fX;
    m[0][1] = fY;
    m[0][2] = fZ;
    m[0][3] = fW;
}

inline void PblMatrix::SetYAxis(float fX, float fY, float fZ, float fW)
{
    m[1][0] = fX;
    m[1][1] = fY;
    m[1][2] = fZ;
    m[1][3] = fW;
}

inline void PblMatrix::SetZAxis(float fX, float fY, float fZ, float fW)
{
    m[2][0] = fX;
    m[2][1] = fY;
    m[2][2] = fZ;
    m[2][3] = fW;
}

inline void PblMatrix::SetTranslation(float fX, float fY, float fZ, float fW)
{
    m[3][0] = fX;
    m[3][1] = fY;
    m[3][2] = fZ;
    m[3][3] = fW;
}
