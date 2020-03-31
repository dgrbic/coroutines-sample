get_filename_component(MY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME CACHE)
project(${MY_NAME})

# set(SOURCE test.cpp)
file(GLOB SOURCES 
    *.cpp
    *.h
)

add_executable(${MY_NAME} ${SOURCES})


