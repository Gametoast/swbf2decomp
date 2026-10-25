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
