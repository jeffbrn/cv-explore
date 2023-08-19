message("==================================================================================")
message(" Preparing Eigen")
message("==================================================================================")

include(FetchContent)
FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        3.4
)

FetchContent_GetProperties(eigen)

if(NOT eigen_POPULATED)
    FetchContent_Populate(eigen)
    add_subdirectory(${eigen_SOURCE_DIR} ${eigen_BINARY_DIR})
endif()

find_package (Eigen3 3.4 REQUIRED NO_MODULE)
