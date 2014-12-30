cmake_minimum_required(VERSION 2.8)
project(zion-ui)
set(VERSION "0.0.0")
set(CMAKE_AUTOMOC TRUE)
find_package(Qt5 REQUIRED
    COMPONENTS Core Gui Declarative OpenGL Quick)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g")


FILE(GLOB helloqml_SRC "src/*.cpp")

QT5_ADD_RESOURCES(helloqml_RESOURCES_RCC resources.qrc)

include_directories(${QT_HEADERS_DIR})
add_definitions(${QT_DEFINITIONS})

add_executable(helloqml ${helloqml_SRC} 
                        draw.c nanovg/nanovg.c
                        ${helloqml_HEADERS_MOC}
                        ${helloqml_RESOURCES_RCC})

TARGET_LINK_LIBRARIES(helloqml ${QT_LIBRARIES} -lm -lGL -lglfw -lGLEW -lyaml-cpp)
qt5_use_modules(helloqml Declarative OpenGL Quick Gui)


add_executable(ui draw.c factory.cpp layout.cpp main.c nanovg/nanovg.c view.cpp)
target_link_libraries(ui -lm -lGL -lglfw -lGLEW -lyaml-cpp)
