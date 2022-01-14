add_library(alhelpers)

target_sources(alhelpers
  PRIVATE
    src/alhelpers.c
    )

target_include_directories(alhelpers
  PRIVATE
    include/alhelpers
  PUBLIC
    include
)

target_link_libraries(alhelpers
  PUBLIC
    OpenAL
)
