#pragma once

#include <sstream>

struct v2
{
    int x;
    int y;
};

struct Dim : v2
{
    int& width;
    int& height;

    Dim() : width(x), height(y)
    {
    }

    Dim(const v2& vec) : v2(vec), width(x), height(y)
    {
    }

    Dim& operator=(const Dim& other)
    {
        if (this != &other)
        {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    Dim& operator=(const v2& vec)
    {
        x = vec.x;
        y = vec.y;
        return *this;
    }
};

inline std::string str(const v2& vec)
{
    std::ostringstream oss;
    oss << "(" << vec.x << ", " << vec.y << ")";
    return oss.str();
}

inline v2 operator*(const v2 vector, const float factor)
{
    return v2{(int)(vector.x * factor), (int)(vector.y * factor)};
}

inline v2 operator*(const v2 vec, const v2 other)
{
    return v2{vec.x * other.x, vec.y * other.y};
}

inline v2 operator/(const v2 vec, const v2 other)
{
    return v2{vec.x / other.x, vec.y / other.y};
}

inline v2 operator/(const v2 vec, const float factor)
{
    return v2{(int)(vec.x / factor), (int)(vec.y / factor)};
}

inline v2 operator+(const v2 vec, const v2 other)
{
    return v2{vec.x + other.x, vec.y + other.y};
}

inline v2 operator-(const v2 vec, const v2 other)
{
    return v2{vec.x - other.x, vec.y - other.y};
}
