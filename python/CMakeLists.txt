cmake_minimum_required(VERSION 3.22) 
project(pybertini)

# All source files to be compiled
# We can either explicitly list all files or use glob better
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


find_package(Boost 1.82 REQUIRED
                COMPONENTS 
                    serialization
                    unit_test_framework
                    filesystem
                    system
                    chrono
                    regex
                    timer
                    log
                    thread
                    log_setup
)

# Find python and Boost - both are required dependencies
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module NumPy)
#find_package(PythonLibs 3.11 REQUIRED)
find_package(Boost COMPONENTS python311 REQUIRED)

# Without this, any build libraries automatically have names "lib{x}.so"
set(CMAKE_SHARED_MODULE_PREFIX "")

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(eigenpy 3.3 REQUIRED CONFIG)
find_package(bertini2 REQUIRED)

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


add_library(_pybertini SHARED ${PYBERTINI_SOURCES} ${PYBERTINI_HEADERS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})
include_directories(${Bertini2_INCLUDES})


target_link_libraries(_pybertini ${Boost_LIBRARIES})
target_link_libraries(_pybertini Eigen3::Eigen)
target_link_libraries(_pybertini mpfr)
target_link_libraries(_pybertini gmp)
target_link_libraries(_pybertini mpc)
target_link_libraries(_pybertini ${Bertini2_LIBRARIES})