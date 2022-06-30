target_sources(DrkCraft
  PRIVATE
    Application.cpp
    Event.cpp
    EventGenerator.cpp
    ImGuiController.cpp
    Layer.cpp
    LayerStack.cpp
)

add_subdirectory(Layers)
