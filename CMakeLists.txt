cmake_minimum_required(VERSION 3.5.0)
project(ThreadPool3.5 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)
add_executable(ThreadPool3.5 ThPool.cpp ReqHandler.cpp main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
