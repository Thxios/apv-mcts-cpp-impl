

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# set (CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed ltorch_cuda -Wl, --as-needed")
list(APPEND CMAKE_PREFIX_PATH "~/libtorch")

project(torch_test)

include_directories(
    Includes
    Sources
)

find_package(Torch REQUIRED)

add_executable(
    torch_test 
    # Tests/torch_test.cpp
    Tests/gomoku_mcts_test.cpp
    Sources/Gomoku/Board.cpp
    Sources/Gomoku/Evaluator.cpp
)
target_link_libraries(torch_test "${TORCH_LIBRARIES}")
set_property(TARGET torch_test PROPERTY CXX_STANDARD 17)


