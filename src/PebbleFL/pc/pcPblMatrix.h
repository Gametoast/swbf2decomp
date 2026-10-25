#pragma once

inline void PblMatrix::SetTranslation(const PblVector3& vTranslation)
{
    *reinterpret_cast<PblVector3*>(m[3]) = vTranslation;
    m[3][3] = 1.0f;
}
