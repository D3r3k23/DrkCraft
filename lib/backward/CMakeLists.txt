option(ENABLE_BACKWARD_OBJ "Enabled backward automatic stacktrace" OFF)
add_library(backward INTERFACE)

target_include_directories(backward
  INTERFACE
    $(CMAKE_CURRENT_SOURCE_DIR)
)

if(${ENABLE_BACKWARD_OBJ})
    target_sources(backward
      INTERFACE
        backward/backward.cpp
    )
endif()
