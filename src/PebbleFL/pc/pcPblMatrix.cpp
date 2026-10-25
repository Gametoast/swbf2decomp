#include <d3dx9math.h>
#include <string.h>

#include "PebbleFL/Common/PblMatrix.h"
#include "PebbleFL/Common/PblVector.h"

// FUNCTION: SWBF2 0x0043a8c0
PblMatrix::PblMatrix(const PblQuaternion& rotation, const PblVector3& translation)
{
    D3DXQUATERNION normalized;
    D3DXMatrixRotationQuaternion((D3DXMATRIX*)this,
        D3DXQuaternionNormalize(&normalized, (const D3DXQUATERNION*)&rotation));
    m[3][0] = translation.x;
    m[3][1] = translation.y;
    m[3][2] = translation.z;
    m[3][3] = 1.0f;
}

// FUNCTION: SWBF2 0x0062c130
PblMatrix::PblMatrix(const PblVector4& x, const PblVector4& y,
                     const PblVector4& z, const PblVector4& t)
{
    m[0][0] = x.x;
    m[0][1] = x.y;
    m[0][2] = x.z;
    m[0][3] = x.w;
    m[1][0] = y.x;
    m[1][1] = y.y;
    m[1][2] = y.z;
    m[1][3] = y.w;
    m[2][0] = z.x;
    m[2][1] = z.y;
    m[2][2] = z.z;
    m[2][3] = z.w;
    m[3][0] = t.x;
    m[3][1] = t.y;
    m[3][2] = t.z;
    m[3][3] = t.w;
}

#pragma warning(disable : 4073)
#pragma init_seg(lib)
// SYNTHETIC: SWBF2 0x0040ae50
// `dynamic initializer for 'gMatrixIdentity''

// GLOBAL: SWBF2 0x009caee0
// gMatrixIdentity
PblMatrix gMatrixIdentity(PblVector4(1.0f, 0.0f, 0.0f, 0.0f), PblVector4(0.0f, 1.0f, 0.0f, 0.0f),
                          PblVector4(0.0f, 0.0f, 1.0f, 0.0f), PblVector4(0.0f, 0.0f, 0.0f, 1.0f));

// FUNCTION: SWBF2 0x004384f0
PblMatrix::PblMatrix(const PblVector3& x, const PblVector3& y,
                     const PblVector3& z, const PblVector3& t)
{
    m[0][0] = x.x;
    m[0][1] = x.y;
    m[0][2] = x.z;
    m[0][3] = 0.0f;
    m[1][0] = y.x;
    m[1][1] = y.y;
    m[1][2] = y.z;
    m[1][3] = 0.0f;
    m[2][0] = z.x;
    m[2][1] = z.y;
    m[2][2] = z.z;
    m[2][3] = 0.0f;
    m[3][0] = t.x;
    m[3][1] = t.y;
    m[3][2] = t.z;
    m[3][3] = 1.0f;
}

// FUNCTION: SWBF2 0x00411dd0
const PblMatrix PblMatrix::GetInverseFast() const
{
    PblMatrix out;

    out = *this;

    float fSwap;
    fSwap = out.m[0][1];
    out.m[0][1] = out.m[1][0];
    out.m[1][0] = fSwap;
    fSwap = out.m[0][2];
    out.m[0][2] = out.m[2][0];
    out.m[2][0] = fSwap;
    fSwap = out.m[1][2];
    out.m[1][2] = out.m[2][1];
    out.m[2][1] = fSwap;

    D3DXVECTOR3 vRotated;
    D3DXVec3TransformNormal(&vRotated,
                            (const D3DXVECTOR3*)out.m[3],
                            (const D3DXMATRIX*)&out);

    out.m[3][0] = -vRotated.x;
    out.m[3][1] = -vRotated.y;
    out.m[3][2] = -vRotated.z;
    out.m[3][3] = 1.0f;

    return out;
}

// FUNCTION: SWBF2 0x00458960
void PblMatrix::Reset()
{
    D3DXMatrixIdentity(reinterpret_cast<D3DXMATRIX*>(this));
}
