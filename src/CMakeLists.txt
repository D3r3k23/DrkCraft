target_sources(DrkCraft
  PRIVATE
    main.cpp
)

target_include_directories(DrkCraft
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(DrkCraft)
