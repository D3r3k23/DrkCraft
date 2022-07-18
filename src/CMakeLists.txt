message("Configuring Argv")
set(ARGV_ENABLE_MAIN)
add_subdirectory(Argv)

target_link_libraries(DrkCraft PUBLIC Argv)
target_compile_definitions(DrkCraft
  PUBLIC
    ARGV_MAIN_NAMESPACE=DrkCraft
)

message("Configuring DrkCraft source")
add_subdirectory(DrkCraft)
