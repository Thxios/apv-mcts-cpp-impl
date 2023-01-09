

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# list(APPEND CMAKE_PREFIX_PATH "~/libtorch")

project(selfplay)

include_directories(
    Includes
)

find_package(Torch REQUIRED)

add_executable(
    selfplay 
    # Tests/torch_test.cpp
    # Tests/gomoku_mcts_test.cpp
    Sources/GomokuSelfplay.cpp
    Sources/MCTS/Node.cpp
    Sources/MCTS/Tree.cpp
    Sources/Gomoku/Board.cpp
    Sources/Gomoku/Evaluator.cpp
    Sources/Gomoku/Server.cpp
    Sources/Gomoku/Logger.cpp
)
target_link_libraries(selfplay "${TORCH_LIBRARIES}")

set_property(TARGET selfplay PROPERTY CXX_STANDARD 17)
if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET selfplay
                        POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${TORCH_DLLS}
                        $<TARGET_FILE_DIR:selfplay>)
endif (MSVC)


