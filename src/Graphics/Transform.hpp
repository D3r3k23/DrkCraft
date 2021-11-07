#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

namespace DrkCraft
{
    class Transform
    {
    private:
        Mat4 matrix;

    public:
        Transform(Mat4 matrix) : matrix(matrix) {}

        static Transform Translation(double x, double y, double z);
        static Transform Scale(double s);
        static Transform Rotation(double theta);
        static Transform Normal(void);

        double* matrix(void) { return matrix.data(); }
    };
}

#endif // TRANSFORM_HPP
