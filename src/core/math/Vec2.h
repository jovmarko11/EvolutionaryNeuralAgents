#ifndef EVOLUTIONARYNEURALAGENTS_VEC2_H
#define EVOLUTIONARYNEURALAGENTS_VEC2_H

#include <cmath>

class Vec2 {

public:

    double x, y;

    Vec2() : x(0), y(0) {}

    Vec2(double x, double y) : x(x), y(y) {}

    double length() const {
        return std::sqrt(x * x + y * y);
    }

    double lengthSquared() const {
        return x * x + y * y;
    }

    Vec2 normalized() const {
        double l = length();

        if (l == 0)
            return Vec2(0, 0);

        return *this / l;
    }

    double dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    double cross(const Vec2& other) const {
        return x * other.y - y * other.x;
    }

    Vec2 rotated(double angleRad) const {
        double c = std::cos(angleRad);
        double s = std::sin(angleRad);
        return Vec2(x * c - y * s, x * s + y * c);
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2 operator+(const Vec2& other) const {
        return {x + other.x, y + other.y};
    }

    Vec2 operator-(const Vec2& other) const {
        return {x - other.x, y - other.y};
    }

    Vec2 operator*(double alpha) const {
        return {x * alpha, y * alpha};
    }

    Vec2 operator/(double alpha) const {
        return {x / alpha, y / alpha};
    }
};

inline Vec2 operator*(double alpha, const Vec2& vector) {
    return vector * alpha;
}

#endif