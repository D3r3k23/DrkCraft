target_sources(DrkCraft
  PRIVATE
    Game.cpp
    GameEvent.cpp
    GameSystem.cpp
    Player.cpp
    SavedGameLoader.cpp
    Sky.cpp
)

add_subdirectory(Entity)
add_subdirectory(Items)
add_subdirectory(Layers)
add_subdirectory(Systems)
add_subdirectory(World)
