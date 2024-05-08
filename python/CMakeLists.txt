cmake_minimum_required(VERSION 3.22) 
project(pybertini)

# In order to find conda, run 'conda activate' and then use 'cmake .. -DCMAKE_PREFIX_PATH=$CONDA_PREFIX' when cmaking

# All source files to be compiled
# We can either explicitly list all files or use glob, we chose to explicitly list files and not to glob
#file(GLOB SOURCES src/*.cpp)

include_directories(include)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")


#Builds a C++ library and the python bindings around it

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(GMP REQUIRED)
find_package(MPFR REQUIRED)
find_package(MPC REQUIRED)

include_directories(${GMP_INCLUDES})
include_directories(${MPC_INCLUDES})

find_package(eigenpy 3.3 REQUIRED CONFIG)


# Find python and Boost - both are required dependencies
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module NumPy)

# Without this, any build libraries automatically have names "lib{x}.so"
set(CMAKE_SHARED_MODULE_PREFIX "")

# this should be OS-specific, as .so is only for macos and unix.
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")


find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Eigenpy finds Boostpython, having our call to find boost libraries before the eigenpy call erases our call, so our call for boost libraries is after this line

find_package(bertini2 REQUIRED)

find_package(Boost 1.82 REQUIRED
                COMPONENTS 
                    serialization
                    wserialization
                    unit_test_framework
                    filesystem
                    system
                    chrono
                    regex
                    timer
                    log
                    thread
                    log_setup
                    python311 # This finds a minimum version
)

set(PYBERTINI_HEADERS
    include/bertini_python.hpp
    include/eigenpy_interaction.hpp
    include/function_tree_export.hpp
    include/mpfr_export.hpp
    include/random_export.hpp
    include/node_export.hpp
    include/symbol_export.hpp
    include/operator_export.hpp
    include/root_export.hpp
    include/system_export.hpp
    include/tracker_export.hpp
    include/endgame_export.hpp
    include/parser_export.hpp
    include/generic_observer.hpp
    include/generic_observable.hpp
    include/tracker_observers.hpp
    include/endgame_observers.hpp
    include/detail.hpp
    include/logging.hpp
)

set(PYBERTINI_SOURCES
    src/eigenpy_interaction.cpp
    src/logging.cpp
    src/detail.cpp
    src/containers.cpp
    src/tracker_export.cpp
    src/endgame_export.cpp
    src/random_export.cpp
    src/mpfr_export.cpp
    src/node_export.cpp
    src/symbol_export.cpp
    src/operator_export.cpp
    src/root_export.cpp
    src/system_export.cpp
    src/parser_export.cpp
    src/generic_observable.cpp
    src/generic_observer.cpp
    src/tracker_observers.cpp
    src/endgame_observers.cpp
    src/zero_dim_export.cpp
    src/bertini_python.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_PHOENIX_STL_TUPLE_H_")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(_pybertini SHARED ${PYBERTINI_SOURCES} ${PYBERTINI_HEADERS})
set_property(TARGET _pybertini PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)

set_target_properties(_pybertini PROPERTIES PREFIX "")
install(TARGETS _pybertini
            DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/pybertini/")

target_link_libraries(_pybertini ${GMP_LIBRARIES})
target_link_libraries(_pybertini ${MPFR_LIBRARIES})
target_link_libraries(_pybertini ${MPC_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})
include_directories(${Bertini2_INCLUDES})

target_link_libraries(_pybertini Eigen3::Eigen)
target_link_libraries(_pybertini eigenpy::eigenpy)
target_link_libraries(_pybertini ${Bertini2_LIBRARIES})
target_link_libraries(_pybertini ${Boost_LIBRARIES})

#include(CMakePrintHelpers)
