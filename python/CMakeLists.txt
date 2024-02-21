cmake_minimum_required(VERSION 3.22) 
project(pybertini)

# All source files to be compiled
# We can either explicitly list all files or use glob better
#file(GLOB SOURCES src/*.cpp)
include_directories(include)


#Builds a C++ library and the python bindings around it

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(MPFR REQUIRED)

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

# NEXT STEP : INSTALL BOOSTPYTHON

# Find python and Boost - both are required dependencies
find_package(Python3 REQUIRED)
find_package(PythonLibs 3.12 REQUIRED)
find_package(Boost COMPONENTS python312 REQUIRED)

# Without this, any build libraries automatically have names "lib{x}.so"
set(CMAKE_SHARED_MODULE_PREFIX "")

find_package(Eigen3 3.3 REQUIRED)
find_package(eigenpy 3.3 REQUIRED)
#find_package(gmp REQUIRED)


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
