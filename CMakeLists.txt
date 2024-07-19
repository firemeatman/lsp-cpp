cmake_minimum_required(VERSION 3.5)
project(LspCPP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(LspBuildTest "build test" ON)

add_subdirectory(lsp)
if(LspBuildTest)
    add_subdirectory(test)
endif()



