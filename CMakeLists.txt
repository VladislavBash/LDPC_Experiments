cmake_minimum_required(VERSION 3.0.0)
project(LDPC_experiment VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
include(CTest)
enable_testing()


# add_library(lib SHARED "libLDPCExperiments.a")
# set_property(TARGET foo PROPERTY
#              IMPORTED_LOCATION "libLDPCExperiments.a")
# add_library(LDPCExperiments STATIC libLDPCExperiments.a)
# add_library(lib_ab library.h)
# add_library(lib_a STATIC libLDPCExperiments.a)

add_executable(LDPC_experiment main.cpp)
find_library(Lib libLDPCExperiments.a HINTS ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(Lib1 PRIVATE Lib "H_with_padding.alist" "H.alist")
# add_library(matrixes STATIC H_with_padding.alist H.alist)
target_link_libraries(LDPC_experiment ${Lib})
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

# target_include_directories(LDPC_experiment PRIVATE sciplot)

# add_executable(main main.cpp)
# set(C_LIB ${CMAKE_BINARY_DIR}/libLDPCExperiments)
# add_library(c STATIC IMPORTED GLOBAL)
# set_target_properties(c
#         PROPERTIES
#         IMPORTED_LOCATION ${C_LIB}
#         )

# target_link_libraries(main c)
# add_library(LDPCExperiments STATIC libLDPCExperiments.a library.h)
# target_link_libraries(LDPCExperiments PRIVATE library.h)
# target_link_libraries(LDPC_experiment PRIVATE LDPCExperiments)

# add_library(LDPCExperiments STATIC IMPORTED)
# add_library(lib1 library.h)
# target_link_libraries(LDPCExperiments INTERFACE library.h)
# add_library(lib2 libLDPCExperiments.a)
# INSTALL(TARGETS LDPCExperiments LDPCExperiments LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
# INSTALL(FILES library.h DESTINATION include/lib)
# target_link_libraries(LDPC_experiment LDPCExperiments)

# add_library(foo STATIC "libLDPCExperiments")
# add_library(lib )
# # set_property(TARGET foo PROPERTY
# #              IMPORTED_LOCATION "libLDPCExperiments.a")
# target_link_libraries(lib foo)
# add_executable(LDPC_experiment main.cpp)
# target_link_libraries(LDPC_experiment lib)


# include(
# "/share/my_library.cmake")
# add_executable(LDPC_experiment main.cpp)
# include_directories("$/include")
# target_link_libraries(LDPC_experiment my_library)

# add_library(exp_lib STATIC library.h)
# include_directories(includes)
# find_library(EXP_LIB lib)
# target_link_libraries(LDPC_experiment LINK_PUBLIC ${EXP_LIB})
# add_library(library STATIC libLDPCExperiments.a)
# add_library(library library.h)
# add_library(LDPCExperiments STATIC libLDPCExperiments.a library.h)
# target_link_libraries(LDPC_experiment LDPCExperiments)
# target_link_libraries(LDPC_library library.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
