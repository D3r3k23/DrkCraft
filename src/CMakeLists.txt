target_sources(DrkCraft
  PRIVATE
    main.cpp
)

target_include_directories(DrkCraft
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Application)
add_subdirectory(Audio)
add_subdirectory(Core)
add_subdirectory(Game)
add_subdirectory(Graphics)
add_subdirectory(System)
add_subdirectory(Util)
