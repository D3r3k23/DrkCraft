add_library(stb_image)
target_sources(stb_image PRIVATE stb_image_impl.cpp)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(stb_image_write)
target_sources(stb_image_write PRIVATE stb_image_write_impl.cpp)
target_include_directories(stb_image_write INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(stb_rect_pack)
target_sources(stb_rect_pack PRIVATE stb_rect_pack_impl.cpp)
target_include_directories(stb_rect_pack INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(stb_truetype)
target_sources(stb_truetype PRIVATE stb_truetype_impl.cpp)
target_include_directories(stb_truetype INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
