

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# list(APPEND CMAKE_PREFIX_PATH "~/libtorch")

project(torch_test)

include_directories(
    Includes
)

find_package(Torch REQUIRED)

add_executable(
    torch_test 
    # Tests/torch_test.cpp
    Tests/gomoku_mcts_test.cpp
    Sources/MCTS/Node.cpp
    Sources/MCTS/Tree.cpp
    Sources/Gomoku/Board.cpp
    Sources/Gomoku/Evaluator.cpp
    Sources/Gomoku/Server.cpp
)
target_link_libraries(torch_test "${TORCH_LIBRARIES}")

set_property(TARGET torch_test PROPERTY CXX_STANDARD 17)
if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET torch_test
                        POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${TORCH_DLLS}
                        $<TARGET_FILE_DIR:torch_test>)
endif (MSVC)


