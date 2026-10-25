#pragma once

// FUNCTION: SWBF2 0x0040ca30
inline const PblVector3 operator+(const PblVector3& lhs, const PblVector3& rhs)
{
    PblVector3 Temp;
    Temp.x = lhs.x + rhs.x;
    Temp.y = lhs.y + rhs.y;
    Temp.z = lhs.z + rhs.z;
    return Temp;
}

// FUNCTION: SWBF2 0x0040ca70
inline const PblVector3 operator-(const PblVector3& lhs, const PblVector3& rhs)
{
    PblVector3 Temp;
    Temp.x = lhs.x - rhs.x;
    Temp.y = lhs.y - rhs.y;
    Temp.z = lhs.z - rhs.z;
    return Temp;
}

// FUNCTION: SWBF2 0x0040cab0
inline const PblVector3 operator*(const PblVector3& lhs, float rhs)
{
    PblVector3 Temp;
    Temp.x = lhs.x * rhs;
    Temp.y = lhs.y * rhs;
    Temp.z = lhs.z * rhs;
    return Temp;
}

// FUNCTION: SWBF2 0x0040caf0
inline const PblVector3 operator/(const PblVector3& lhs, float rhs)
{
    return lhs * (1.0f / rhs);
}

// FUNCTION: SWBF2 0x00410b60
inline float DotProduct(const PblVector3& vVector1, const PblVector3& vVector2)
{
    return vVector1.y * vVector2.y + vVector1.x * vVector2.x + vVector1.z * vVector2.z;
}

inline PblVector4::operator const PblVector3&() const
{
    return *reinterpret_cast<const PblVector3*>(this);
}

// FUNCTION: SWBF2 0x004213a0
inline const float Length(const PblVector3& vVector)
{
    return sqrtf(vVector.x * vVector.x + vVector.y * vVector.y + vVector.z * vVector.z);
}

inline const float SquaredLength(const PblVector3& vVector)
{
    return vVector.x * vVector.x + vVector.y * vVector.y + vVector.z * vVector.z;
}

inline PblVector3& PblVector3::operator+=(const PblVector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

inline const PblVector3 PblVector3::operator-() const
{
    return PblVector3(-x, -y, -z);
}

inline PblVector3& PblVector3::operator-=(const PblVector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

// FUNCTION: SWBF2 0x004171f0
inline const PblVector3 CrossProduct(const PblVector3& lhs, const PblVector3& rhs)
{
    PblVector3 Temp;
    Temp.x = lhs.y * rhs.z - lhs.z * rhs.y;
    Temp.y = lhs.z * rhs.x - lhs.x * rhs.z;
    Temp.z = lhs.x * rhs.y - lhs.y * rhs.x;
    return Temp;
}
