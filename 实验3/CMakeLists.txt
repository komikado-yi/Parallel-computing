cmake_minimum_required(VERSION 3.19)
project(Parallel_Programming)

enable_language(Fortran)

find_package(MPI)
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(SOURCE_FILES "Parallel Monte Carlo Operation Reduced Version")
add_executable(Parallel_Programming ${SOURCE_FILES})
target_compile_options(Parallel_Programming PRIVATE ${MPI_CXX_COMPILE_FLAGS})
target_link_libraries(Parallel_Programming ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS})