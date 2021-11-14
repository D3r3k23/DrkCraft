target_sources(DrkCraft
  PRIVATE
    main.cpp
)

target_include_directories(DrkCraft
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Application)
add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Game)
add_subdirectory(Graphics)
