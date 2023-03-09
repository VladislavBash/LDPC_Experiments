cmake_minimum_required(VERSION 3.0.0)
project(LDPC_experiment VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
include(CTest)
enable_testing()


add_executable(LDPC_experiment main.cpp)
find_library(Lib libLDPCExperiments.a HINTS ${CMAKE_CURRENT_SOURCE_DIR})

find_package(MPI REQUIRED)

target_link_libraries(LDPC_experiment ${Lib} MPI::MPI_CXX)


add_custom_command(
      TARGET LDPC_experiment POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
              ${CMAKE_SOURCE_DIR}/H.alist
              ${CMAKE_BINARY_DIR}/H.alist
)
add_custom_command(
      TARGET LDPC_experiment POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
              ${CMAKE_SOURCE_DIR}/H_with_padding.alist
              ${CMAKE_BINARY_DIR}/H_with_padding.alist
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)