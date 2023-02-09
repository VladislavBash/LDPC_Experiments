cmake_minimum_required(VERSION 3.0.0)
project(LDPC_experiment VERSION 0.1.0)

include(CTest)
enable_testing()


# add_library(foo STATIC "libLDPCExperiments.a")
# set_property(TARGET foo PROPERTY
#              IMPORTED_LOCATION "libLDPCExperiments.a")
add_executable(LDPC_experiment main.cpp)
target_link_libraries(LDPC_experiment library.h libLDPCExperiments.a)

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
