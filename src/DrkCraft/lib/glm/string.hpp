#ifndef DRK_LIB_GLM_STRING_HPP
#define DRK_LIB_GLM_STRING_HPP

#include "Lib/glm/vec.hpp"
#include "Lib/glm/mat.hpp"

#include <glm/gtx/string_cast.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <string>
#include <ostream>

namespace DrkCraft
{
    template <int N, typename T, glm::qualifier Q>
    std::string to_string(const glm::vec<N, T, Q>& v)
    {
        return glm::to_string(v);
    }

    template <int N, typename T, glm::qualifier Q>
    std::ostream& operator<<(std::ostream& os, const glm::vec<N, T, Q>& v)
    {
        return os << to_string(v);
    }

    template <int N, typename T, glm::qualifier Q>
    std::string to_string(const glm::mat<N, N, T, Q>& m)
    {
        return glm::to_string(m);
    }

    template <int N, typename T, glm::qualifier Q>
    std::ostream& operator<<(std::ostream& os, const glm::mat<N, N, T, Q>& m)
    {
        return os << to_string(m);
    }

}

#endif // DRK_LIB_GLM_STRING_HPP
