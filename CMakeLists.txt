cmake_minimum_required(VERSION 2.8)
project(zion-ui)
set(VERSION "0.0.0")
set(CMAKE_AUTOMOC TRUE)
find_package(Qt5 REQUIRED
    COMPONENTS Core Gui Declarative OpenGL Quick)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g")

set(helloqml_SRC     src/zKnob.cpp src/zGraph.cpp src/zmain.cpp draw.c nanovg/nanovg.c)
set(helloqml_HEADERS src/zKnob.h src/zGraph.h)

#QT4_WRAP_CPP(helloqml_HEADERS_MOC ${helloqml_HEADERS})
QT5_ADD_RESOURCES(helloqml_RESOURCES_RCC resources.qrc)

#include(${QT_USE_FILE})
include_directories(${QT_HEADERS_DIR})
add_definitions(${QT_DEFINITIONS})

add_executable(helloqml ${helloqml_SRC} 
                        ${helloqml_HEADERS_MOC}
                        ${helloqml_RESOURCES_RCC})

TARGET_LINK_LIBRARIES(helloqml ${QT_LIBRARIES} -lm -lGL -lglfw -lGLEW -lyaml-cpp)
qt5_use_modules(helloqml Declarative OpenGL Quick Gui)


add_executable(ui draw.c factory.cpp layout.cpp main.c nanovg/nanovg.c view.cpp)
target_link_libraries(ui -lm -lGL -lglfw -lGLEW -lyaml-cpp)
