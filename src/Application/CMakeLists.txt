target_sources(DrkCraft
  PRIVATE
    Application.cpp
    Event.cpp
    EventGenerator.cpp
    Layer.cpp
    LayerStack.cpp
    Timestep.cpp
)

add_subdirectory(Layers)
