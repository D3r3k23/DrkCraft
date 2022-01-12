add_library(stb_image)
target_sources(stb_image PRIVATE stb_image_impl.cpp)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(stb_rect_pack)
target_sources(stb_rect_pack PRIVATE stb_rect_pack_impl.cpp)
target_include_directories(stb_rect_pack PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(stb_truetype)
target_sources(stb_truetype PRIVATE stb_truetype_impl.cpp)
target_include_directories(stb_truetype PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
