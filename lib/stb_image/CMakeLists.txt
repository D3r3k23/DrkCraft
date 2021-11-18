add_library(stb_image)
target_sources(stb_image PRIVATE stb_image_impl.cpp)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
