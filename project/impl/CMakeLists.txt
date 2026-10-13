cmake_minimum_required(VERSION 3.20)
project(open_game_library CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ogl STATIC
    src/comment.cpp
    src/game.cpp
    src/repository.cpp
    src/request.cpp
    src/review.cpp
    src/user.cpp
)
target_include_directories(ogl PUBLIC include)

add_executable(demo src/demo.cpp)
target_link_libraries(demo PRIVATE ogl)

add_executable(scenario_nedlastning scenarios/nedlastning.cpp)
target_link_libraries(scenario_nedlastning PRIVATE ogl)
add_executable(scenario_publish scenarios/publish.cpp)
target_link_libraries(scenario_publish PRIVATE ogl)
add_executable(scenario_request scenarios/request.cpp)
target_link_libraries(scenario_request PRIVATE ogl)
add_executable(scenario_review scenarios/review.cpp)
target_link_libraries(scenario_review PRIVATE ogl)

foreach(target ogl demo scenario_nedlastning scenario_publish scenario_request scenario_review)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
endforeach()
