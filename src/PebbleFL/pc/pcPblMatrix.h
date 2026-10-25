#pragma once

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
