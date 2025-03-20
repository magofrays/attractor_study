#include "vector3.h"
#include <vector>

struct Matrix3x3
{
    std::vector<std::vector<double>> M;
    Matrix3x3() : M(3, std::vector<double>(3)) {}
    double find_determinant()
    {
        double det = M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
                     M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]) +
                     M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]);
        return det;
    }
    Matrix3x3 find_adj()
    {
        Matrix3x3 adj;
        adj.M[0][0] = (M[1][1] * M[2][2] - M[1][2] * M[2][1]);
        adj.M[0][1] = -(M[0][1] * M[2][2] - M[0][2] * M[2][1]);
        adj.M[0][2] = (M[0][1] * M[1][2] - M[0][2] * M[1][1]);

        adj.M[1][0] = -(M[1][0] * M[2][2] - M[1][2] * M[2][0]);
        adj.M[1][1] = (M[0][0] * M[2][2] - M[0][2] * M[2][0]);
        adj.M[1][2] = -(M[0][0] * M[1][2] - M[0][2] * M[1][0]);

        adj.M[2][0] = (M[1][0] * M[2][1] - M[1][1] * M[2][0]);
        adj.M[2][1] = -(M[0][0] * M[2][1] - M[0][1] * M[2][0]);
        adj.M[2][2] = (M[0][0] * M[1][1] - M[0][1] * M[1][0]);
        return adj;
    }
    Matrix3x3 find_inverse()
    {
        double det = find_determinant();
        Matrix3x3 inv = find_adj();
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                inv.M[i][j] = inv.M[i][j] / det;
            }
        }
        return inv;
    }
    std::vector<double> &operator[](size_t index)
    {
        if (index >= 3)
        {
            throw std::out_of_range("Index out of range [0, 2]");
        }
        return M[index];
    }
    vec3 operator*(const vec3 &v) const
    {
        return vec3(
            M[0][0] * v.x + M[0][1] * v.y + M[0][2] * v.z,
            M[1][0] * v.x + M[1][1] * v.y + M[1][2] * v.z,
            M[2][0] * v.x + M[2][1] * v.y + M[2][2] * v.z);
    }
};