#include "World.hpp"

namespace DrkCraft::Game
{
    const std::vector<Chunk>& World::get_chunks(void) const
    {
        return m_chunks;
    }
}
