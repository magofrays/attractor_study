#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>

struct vec3
{
    double x;
    double y;
    double z;

    vec3(double x, double y, double z) : x(x), y(y), z(z) {}
    vec3() : x(0), y(0), z(0) {};
    vec3 operator+(const vec3 &point) const
    {
        return vec3(x + point.x, y + point.y, z + point.z);
    }
    vec3 &operator+=(const vec3 &point)
    {
        x += point.x;
        y += point.y;
        z += point.z;
        return *this;
    }
    vec3 operator*(double scalar) const
    {
        return vec3(x * scalar, y * scalar, z * scalar);
    }

    vec3 operator/(double scalar) const
    {
        if (scalar == 0)
        {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
        return vec3(x / scalar, y / scalar, z / scalar);
    }

    friend std::ostream &operator<<(std::ostream &os, const vec3 &v)
    {
        os << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")\n";
        return os;
    }
    double distance()
    {
        return sqrt(x * x + y * y + z * z);
    }
};

#endif