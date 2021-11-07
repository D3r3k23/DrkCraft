#ifndef MATH_HPP
#define MATH_HPP

namespace DrkCraft
{
    struct Vec2
    {
        double x;
        double y;
    };

    struct Vec3
    {
        double x;
        double y;
        double z;
    };

    struct Mat3
    {
        double m[3][3];
    };

    struct Mat4
    {
        double m[4][4];

        double* data(void) { return &m[0][0]; }
    };
}

#endif // MATH_HPP
