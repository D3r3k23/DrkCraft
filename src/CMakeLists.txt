target_sources(DrkCraft
  PUBLIC
    main.cpp
)

target_include_directories(.)
add_subdirectory(Application)
add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Game)
add_subdirectory(Graphics)
