cmake_minimum_required(VERSION 3.12)

add_subdirectory(./simulator ${CMAKE_CURRENT_SOURCE}/./simulator/build)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(wobbler2 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()





add_executable(${PROJECT_NAME}
    Wobbler2Main.cpp
    pendulum.cpp
    Wobbler2.cpp
)

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/spi.pio)



pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}
    pico_stdlib hardware_adc hardware_pio 
) 