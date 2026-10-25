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
