add_library(minimp3)
target_sources(minimp3 PRIVATE minimp3_impl.cpp)
target_include_directories(minimp3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
