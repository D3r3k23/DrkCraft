#include "ChunkManager.hpp"

namespace DrkCraft
{
    constexpr uint MAX_LOADED_CHUNK_DISTANCE = 16;
    constexpr uint MAX_LOADED_CHUNKS_WIDTH = MAX_LOADED_CHUNK_DISTANCE * 2;
    constexpr uint MAX_LOADED_CHUNKS     = MAX_LOADED_CHUNKS_WIDTH * MAX_LOADED_CHUNKS_WIDTH;

    static Ref<Chunk> s_loadedChunks[MAX_LOADED_CHUNKS_WIDTH][MAX_LOADED_CHUNKS_WIDTH];
}
