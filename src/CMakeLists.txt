target_sources(DrkCraft
  PRIVATE
    main.cpp
)

add_subdirectory(DrkCraft)

add_subdirectory(Argv)
target_link_libraries(DrkCraft
  PUBLIC
    Argv
)
