#include "Math.hpp"

#include "Lib/glm/vec4.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace DrkCraft::Math
{
#if 0
    MatrixDecomposition decompose_matrix(const mat4 matrix)
    {
        vec3 scale;
        glm::quat rotation;
        vec3 translation;
        vec3 skew;
        vec4 perspective;
        glm::decompose(matrix, scale, rotation, translation, skew, perspective);
        return { translation, glm::conjugate(rotation), scale };
    }
#endif
}
