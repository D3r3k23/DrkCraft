#include "Util.hpp"

namespace DrkCraft
{
    vec3 unit_vector(Direction direction)
    {
        using enum Direction;
        switch (direction)
        {
            case Left     : return { -1.0f,  0.0f,  0.0f };
            case Right    : return {  1.0f,  0.0f,  0.0f };
            case Forward  : return {  0.0f,  0.0f, -1.0f };
            case Backward : return {  0.0f,  0.0f,  1.0f };
            case Up       : return {  0.0f,  1.0f,  0.0f };
            case Down     : return {  0.0f, -1.0f,  0.0f };
            default:
                DRK_ASSERT_DEBUG_NO_MSG(false);
                return { 0.0f, 0.0f, 0.0f };
        }
    }
}
