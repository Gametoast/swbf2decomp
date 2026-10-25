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
