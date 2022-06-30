#ifndef DRK_GAME_UTIL_UUID_HPP
#define DRK_GAME_UTIL_UUID_HPP

#include "Core/Base.hpp"

namespace DrkCraft::Game
{
    class UUID
    {
    public:
        UUID(void);

        UUID(uint64 uuid)
          : m_uuid(uuid)
        { }

        UUID(const UUID& other) = default;

        friend bool operator==(const UUID& uuid1, const UUID& uuid2)
        {
            return uuid1.m_uuid == uuid2.m_uuid;
        }

    private:
        uint64 m_uuid;
    };
}

#endif // DRK_GAME_UTIL_UUID_HPP
