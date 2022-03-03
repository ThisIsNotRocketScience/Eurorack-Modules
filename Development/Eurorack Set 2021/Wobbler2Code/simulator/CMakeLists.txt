cmake_minimum_required(VERSION 3.7)

project(Wobbler2Simulator 
LANGUAGES CXX
VERSION 1.0
)


add_subdirectory(dependencies)

add_executable(Wobbler2Simulator wobbler2_simulator_main.cpp)
target_link_libraries(Wobbler2Simulator PRIVATE ImGui-SFML::ImGui-SFML  sfml-audio)

# Copy DLLs needed for runtime on Windows
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET Wobbler2Simulator POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-audio>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE_DIR:Wobbler2Simulator>)
  endif()
endif()