cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

project(Cv_Test VERSION 1.0.0)

include(cmake/eigen.cmake)
include(cmake/fftw.cmake)
include(cmake/debug.cmake)

add_executable(cv1 main.cpp
    src/image/image.cpp
    src/filters/fft.cpp
)
target_include_directories(cv1 PUBLIC include PRIVATE ${fftw_SOURCE_DIR}/api)
target_link_libraries(cv1 Eigen3::Eigen fftw3)
