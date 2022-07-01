add_subdirectory(DrkCraft)

set(ARGV_ENABLE_MAIN)
add_subdirectory(Argv)
target_compile_definitions(DrkCraft
  PUBLIC
    ARGV_MAIN_NAMESPACE=DrkCraft
)
target_link_libraries(DrkCraft PUBLIC Argv)
