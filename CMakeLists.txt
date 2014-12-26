cmake_minimum_required(VERSION 2.8)
project(zion-ui)
set(VERSION "0.0.0")
find_package(Qt4 REQUIRED
    COMPONENTS QtCore QtGui QtDeclarative QtOpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g")

set(helloqml_SRC     src/zKnob.cpp src/zGraph.cpp src/zmain.cpp draw.c nanovg/nanovg.c)
set(helloqml_HEADERS src/zKnob.h src/zGraph.h)

QT4_WRAP_CPP(helloqml_HEADERS_MOC ${helloqml_HEADERS})
QT4_ADD_RESOURCES(helloqml_RESOURCES_RCC resources.qrc)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(helloqml ${helloqml_SRC} 
                        ${helloqml_HEADERS_MOC}
                        ${helloqml_RESOURCES_RCC})

TARGET_LINK_LIBRARIES(helloqml ${QT_LIBRARIES} -lm -lGL -lglfw -lGLEW -lyaml-cpp)


add_executable(ui draw.c factory.cpp layout.cpp main.c nanovg/nanovg.c view.cpp)
target_link_libraries(ui -lm -lGL -lglfw -lGLEW -lyaml-cpp)
