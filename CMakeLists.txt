cmake_minimum_required(VERSION 3.13..3.24 FATAL_ERROR)
## Project name
set(FrameWorkName "tlux")
project(tlux  VERSION 0.0.1 LANGUAGES CXX)


include(tlux.cmake)

enable_testing()

add_subdirectory(tests)
