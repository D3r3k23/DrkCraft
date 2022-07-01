add_subdirectory(DrkCraft)

add_subdirectory(Argv)
target_compile_definitions(Argv
  INTERFACE
    ARGV_MAIN_NAMESPACE=DrkCraft
)
target_link_libraries(DrkCraft PUBLIC Argv)
