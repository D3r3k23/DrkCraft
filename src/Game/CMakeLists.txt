target_sources(DrkCraft
  PRIVATE
    Block.cpp
    Chunk.cpp
    ChunkManager.cpp
    ChunkRenderer.cpp
    Components.cpp
    Entity.cpp
    Game.cpp
    GameEvent.cpp
    Player.cpp
    SavedWorldLoader.cpp
    World.cpp
    WorldGenerator.cpp
)

add_subdirectory(Layers)
