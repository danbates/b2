cmake_minimum_required(VERSION 3.22) 
project(bertini2)

# All source files to be compiled
# We can either explicitly list all files or use glob better
#file(GLOB SOURCES src/*.cpp)
include_directories(include)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")

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

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

configure_file(config.h.in include/bertini2/config.h)

# This was from a makemodule.am, take the lists and define them in seperate blocks with the variables they already have, then add the variable name to library_headrs
set(endgames_headers
	include/bertini2/endgames/amp_endgame.hpp
	include/bertini2/endgames/base_endgame.hpp
	include/bertini2/endgames/cauchy.hpp
	include/bertini2/endgames/config.hpp
	include/bertini2/endgames/events.hpp
	include/bertini2/endgames/fixed_prec_endgame.hpp
	include/bertini2/endgames/interpolation.hpp
	include/bertini2/endgames/observers.hpp
	include/bertini2/endgames/powerseries.hpp
	include/bertini2/endgames/prec_base.hpp
)

set (basics_rootinclude_headers
    include/bertini2/have_bertini.hpp
    include/bertini2/mpfr_extensions.hpp
    include/bertini2/mpfr_complex.hpp
    include/bertini2/forbid_mixed_arithmetic.hpp
    include/bertini2/double_extensions.hpp
    include/bertini2/random.hpp
    include/bertini2/num_traits.hpp
    include/bertini2/classic.hpp
    include/bertini2/eigen_extensions.hpp
    include/bertini2/eigen_serialization_addon.hpp
    include/bertini2/logging.hpp
    include/bertini2/endgames.hpp
    "${CMAKE_CURRENT_BINARY_DIR}/include/bertini2/config.h"
    include/bertini2/bertini.hpp
)

set (common_headers
    include/bertini2/common/config.hpp 
    include/bertini2/common/stream_enum.hpp
)

set(detail_headers
    include/bertini2/detail/configured.hpp
    include/bertini2/detail/events.hpp
    include/bertini2/detail/visitable.hpp
    include/bertini2/detail/visitor.hpp
    include/bertini2/detail/observer.hpp
    include/bertini2/detail/observable.hpp
    include/bertini2/detail/is_template_parameter.hpp
    include/bertini2/detail/enable_permuted_arguments.hpp
    include/bertini2/detail/typelist.hpp
)

set(function_tree_headers
    include/bertini2/function_tree.hpp
    include/bertini2/function_tree/node.hpp
    include/bertini2/function_tree/forward_declares.hpp
    include/bertini2/function_tree/simplify.hpp
    include/bertini2/function_tree/operators/operator.hpp
    include/bertini2/function_tree/symbols/symbol.hpp
    include/bertini2/function_tree/symbols/variable.hpp
    include/bertini2/function_tree/symbols/differential.hpp
    include/bertini2/function_tree/symbols/special_number.hpp
    include/bertini2/function_tree/symbols/number.hpp
    include/bertini2/function_tree/symbols/linear_product.hpp
    include/bertini2/function_tree/roots/function.hpp
    include/bertini2/function_tree/roots/jacobian.hpp
    include/bertini2/function_tree/operators/arithmetic.hpp
    include/bertini2/function_tree/operators/trig.hpp
)

set(function_tree_headers_rootinclude_HEADERS
	include/bertini2/function_tree.hpp
)

set(settings_headers
    include/bertini2/settings/configIni_parse.hpp
)

set(functiontreeinclude_HEADERS
	include/bertini2/function_tree/node.hpp 
	include/bertini2/function_tree/factory.hpp 
	include/bertini2/function_tree/forward_declares.hpp 
	include/bertini2/function_tree/simplify.hpp
)

set(functiontree_operators_HEADERS
	include/bertini2/function_tree/operators/operator.hpp 
	include/bertini2/function_tree/operators/arithmetic.hpp 
	include/bertini2/function_tree/operators/trig.hpp
)

set(functiontree_symbols_HEADERS
	include/bertini2/function_tree/symbols/symbol.hpp 
	include/bertini2/function_tree/symbols/variable.hpp 
	include/bertini2/function_tree/symbols/differential.hpp 
	include/bertini2/function_tree/symbols/special_number.hpp 
	include/bertini2/function_tree/symbols/number.hpp
    include/bertini2/function_tree/symbols/linear_product.hpp
)

set(functiontree_roots_HEADERS
	include/bertini2/function_tree/roots/function.hpp 
	include/bertini2/function_tree/roots/jacobian.hpp
)

set(io_headers
    include/bertini2/io/file_utilities.hpp
    include/bertini2/io/generators.hpp
    include/bertini2/io/parsing.hpp
    include/bertini2/io/splash.hpp
)

set (io_parsing_headers
    include/bertini2/io/parsing/classic_utilities.hpp
    include/bertini2/io/parsing/function_parsers.hpp
    include/bertini2/io/parsing/function_rules.hpp
    include/bertini2/io/parsing/number_parsers.hpp
    include/bertini2/io/parsing/number_rules.hpp
    include/bertini2/io/parsing/qi_files.hpp
    include/bertini2/io/parsing/settings_parsers.hpp
    include/bertini2/io/parsing/settings_rules.hpp
    include/bertini2/io/parsing/system_parsers.hpp
    include/bertini2/io/parsing/system_rules.hpp 
)

set (io_parsing_settings_headers
    include/bertini2/io/parsing/settings_parsers/algorithm.hpp
    include/bertini2/io/parsing/settings_parsers/base.hpp
    include/bertini2/io/parsing/settings_parsers/endgames.hpp
    include/bertini2/io/parsing/settings_parsers/tracking.hpp
)

set(nag_algorithms_headers
	include/bertini2/nag_algorithms/midpath_check.hpp
	include/bertini2/nag_algorithms/numerical_irreducible_decomposition.hpp
	include/bertini2/nag_algorithms/output.hpp
	include/bertini2/nag_algorithms/sharpen.hpp
	include/bertini2/nag_algorithms/trace.hpp
	include/bertini2/nag_algorithms/zero_dim_solve.hpp 
)

set(nag_algorithms_common_headers
    include/bertini2/nag_algorithms/common/algorithm_base.hpp 
    include/bertini2/nag_algorithms/common/config.hpp 
    include/bertini2/nag_algorithms/common/policies.hpp
)

set(nag_datatypes_headers
	include/bertini2/nag_datatypes/numerical_irreducible_decomposition.hpp
	include/bertini2/nag_datatypes/witness_set.hpp
)

set(nag_datatypes_common_headers
	include/bertini2/nag_datatypes/common/policies.hpp
)

set(parallel_headers
    include/bertini2/parallel/initialize_finalize.hpp
)

set(parallel_rootinclude_HEADERS
    include/bertini2/parallel.hpp
)

set(pool_headers
    include/bertini2/pool/pool.hpp 
    include/bertini2/pool/system.hpp 
)

set(system_headers
	include/bertini2/system/patch.hpp
	include/bertini2/system/precon.hpp
	include/bertini2/system/slice.hpp
	include/bertini2/system/start_base.hpp
	include/bertini2/system/start_systems.hpp
	include/bertini2/system/straight_line_program.hpp
	include/bertini2/system/system.hpp
)

set(system_start_headers
	include/bertini2/system/start/total_degree.hpp
	include/bertini2/system/start/mhom.hpp
	include/bertini2/system/start/user.hpp
	include/bertini2/system/start/utility.hpp
)

set(system_rootinclude_HEADERS
	include/bertini2/system.hpp
)

set(trackers_HEADERS
	include/bertini2/trackers/adaptive_precision_utilities.hpp
	include/bertini2/trackers/amp_criteria.hpp
	include/bertini2/trackers/amp_tracker.hpp
	include/bertini2/trackers/base_predictor.hpp
	include/bertini2/trackers/base_tracker.hpp
	include/bertini2/trackers/events.hpp
	include/bertini2/trackers/explicit_predictors.hpp
	include/bertini2/trackers/fixed_precision_tracker.hpp
	include/bertini2/trackers/fixed_precision_utilities.hpp
	include/bertini2/trackers/observers.hpp
	include/bertini2/trackers/ode_predictors.hpp
	include/bertini2/trackers/predict.hpp
	include/bertini2/trackers/step.hpp
	include/bertini2/trackers/tracker.hpp
	include/bertini2/trackers/config.hpp
)

set(tracking_rootinclude_HEADERS
	include/bertini2/tracking.hpp
)

set(BERTINI2_LIBRARY_HEADERS
    ${endgames_headers}
    ${basics_headers}
    ${common_headers}
    ${details_headers}
    ${function_tree_headers}
    ${io_headers}
    ${io_parsing_headers}
    ${io_parsing_settings_headers}
    ${nag_algorithms_headers}
    ${nag_algorithms_common_headers}
    ${nag_datatypes_headers}
    ${nag_datatypes_common_headers}
    ${parallel_headers}
    ${parallel_rootinclude_HEADERS}
    ${pool_headers}
    ${system_headers}
    ${system_start_headers}
    ${system_rootinclude_HEADERS}
    ${systeminclude_HEADERS}
    ${startinclude_HEADERS}
    ${trackers_HEADERS}
    ${tracking_header_files}
    ${tracking_rootinclude_HEADERS}
    ${trackersinclude_HEADERS}
)

set(basics_sources
    src/basics/random.cpp 
    src/basics/have_bertini.cpp
)

set(function_tree_sources
    src/function_tree/node.cpp
    src/function_tree/simplify.cpp
    src/function_tree/operators/arithmetic.cpp
    src/function_tree/operators/trig.cpp
    src/function_tree/linear_product.cpp
    src/function_tree/operators/operator.cpp
    src/function_tree/symbols/special_number.cpp
    src/function_tree/symbols/differential.cpp
    src/function_tree/symbols/symbol.cpp
    src/function_tree/symbols/variable.cpp
    src/function_tree/symbols/number.cpp
    src/function_tree/roots/jacobian.cpp
    src/function_tree/roots/function.cpp
)

set(parallel_sources
    src/parallel/parallel.cpp 
    src/parallel/initialize_finalize.cpp
)

set(system_source_files
    src/system/precon.cpp
    src/system/slice.cpp
    src/system/start_base.cpp
    src/system/system.cpp
    src/system/straight_line_program.cpp
    src/system/start/total_degree.cpp
    src/system/start/mhom.cpp
    src/system/start/user.cpp
)

set(tracking_source_files
    src/tracking/explicit_predictors.cpp
)

set(BERTINI2_LIBRARY_SOURCES
    ${basics_sources}
    ${libbertini2_la_SOURCES}
    ${function_tree_sources}
    ${parallel_sources}
    ${system_source_files}
    ${tracking_source_files}
)

set(BERTINI2_EXE_SOURCES 
    src/blackbox/bertini.cpp 
    src/blackbox/main_mode_switch.cpp 
    src/blackbox/argc_argv.cpp
)

set(BERTINI2_EXE_HEADERS 
    include/bertini2/blackbox/main_mode_switch.hpp 
    include/bertini2/blackbox/argc_argv.hpp 
    include/bertini2/blackbox/config.hpp
    include/bertini2/blackbox/algorithm_builder.hpp
    include/bertini2/blackbox/global_configs.hpp
    include/bertini2/blackbox/switches_zerodim.hpp
    include/bertini2/blackbox/user_homotopy.hpp
)

add_executable(bertini2_exe ${BERTINI2_EXE_SOURCES} ${BERTINI2_EXE_HEADERS})
set_property(TARGET bertini2_exe PROPERTY OUTPUT_NAME bertini2)

# All library files
add_library(bertini2 
    ${BERTINI2_LIBRARY_SOURCES}
    ${BERTINI2_LIBRARY_HEADERS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_PHOENIX_STL_TUPLE_H_")


target_link_libraries(bertini2 ${GMP_LIBRARIES})
target_link_libraries(bertini2 ${MPFR_LIBRARIES})
target_link_libraries(bertini2 ${MPC_LIBRARIES})
target_link_libraries(bertini2 ${Boost_LIBRARIES})
target_link_libraries(bertini2 Eigen3::Eigen)
target_link_libraries(bertini2_exe ${Boost_LIBRARIES} bertini2)


# Compile flags	include/bertini2/common/config.hpp \
#	include/bertini2/common/stream_enum.hpp

target_compile_options(bertini2 PRIVATE -Wall -Wextra)


install( 
    TARGETS bertini2
    ARCHIVE DESTINATION "lib"
    LIBRARY DESTINATION "lib"
    COMPONENT library
) 

install(
    FILES $(BERTINI2_EXE_HEADERS)
    DESTINATION "include/bertini2/blackbox"
)

install(
    FILES ${function_tree_headers_rootinclude_HEADERS} ${parallel_rootinclude_HEADERS} ${system_rootinclude_HEADERS} ${tracking_rootinclude_HEADERS} ${parallel_rootinclude_HEADERS} ${system_rootinclude_HEADERS} ${tracking_rootinclude_HEADERS}
    DESTINATION "include/bertini2"
)

install(
    FILES ${endgames_headers}
    DESTINATION "include/bertini2/endgames"
) 

install(
    FILES ${trackers_headers}
    DESTINATION "include/bertini2/trackers"
) 

install(
    FILES ${basics_rootinclude_headers}
    DESTINATION "include/bertini2"
) 

install(
    FILES ${common_headers}
    DESTINATION "include/bertini2/common"
) 

install(
    FILES ${detail_headers}
    DESTINATION "include/bertini2/detail"
) 

install(
    FILES ${function_tree_headers}
    DESTINATION "include/bertini2/function_tree"
) 

install(
    FILES ${function_tree_headers_rootinclude_HEADERS}#
    DESTINATION "include/bertini2"
) 

install(
    FILES ${functiontree_operators_HEADERS}
    DESTINATION "include/bertini2/function_tree/operators"
) 

install(
    FILES ${functiontree_symbols_HEADERS}
    DESTINATION "include/bertini2/function_tree/symbols"
) 

install(
    FILES ${functiontree_roots_HEADERS}
    DESTINATION "include/bertini2/function_tree/roots/"
) 

install(
    FILES ${io_headers}
    DESTINATION "include/bertini2/io"
) 

install(
    FILES ${io_parsing_headers}
    DESTINATION "include/bertini2/io/parsing"
) 

install(
    FILES ${io_parsing_settings_headers}
    DESTINATION "include/bertini2/io/parsing/settings_parsers"
) 

install(
    FILES ${nag_algorithms_headers}
    DESTINATION "include/bertini2/nag_algorithms"
) 

install(
    FILES ${nag_algorithms_common_headers}
    DESTINATION "include/bertini2/nag_algorithms/common"
) 

install(
    FILES ${nag_datatypes_headers}
    DESTINATION "include/bertini2/nag_datatypes"
) 

install(
    FILES ${nag_datatypes_common_headers}
    DESTINATION "include/bertini2/nag_datatypes/common"
) 

install(
    FILES ${parallel_headers}
    DESTINATION "include/bertini2/parallel"
) 

install(
    FILES ${parallel_rootinclude_HEADERS}
    DESTINATION "include/bertini2"
) 

install(
    FILES ${pool_headers}
    DESTINATION "include/bertini2/pool"
) 

install(
    FILES ${system_headers}
    DESTINATION "include/bertini2/system"
) 

install(
    FILES ${system_start_headers}
    DESTINATION "include/bertini2/system/start"
) 

install(
    FILES ${system_rootinclude_HEADERS}
    DESTINATION "include/bertini2"
) 

install(
    FILES ${tracking_rootinclude_HEADERS}
    DESTINATION "include/bertini2"
) 

install(
    FILES ${trackers_HEADERS}
    DESTINATION "include/bertini2/trackers"
) 

install(
    FILES ${settings_headers}
    DESTINATION "include/bertini2/settings"
)

install(
    FILES ${tracking_header_files}
    DESTINATION "include/bertini2/tracking"
) 

install(
    FILES ${system_source_files}
    DESTINATION "include/bertini2/system"
) 

set(B2_CLASS_TEST_SOURCES
    test/classes/boost_multiprecision_test.cpp 
    test/classes/fundamentals_test.cpp 
    test/classes/eigen_test.cpp 
    test/classes/complex_test.cpp 
    test/classes/function_tree_test.cpp 
    test/classes/function_tree_transform.cpp 
    test/classes/system_test.cpp 
    test/classes/slp_test.cpp 
    test/classes/differentiate_test.cpp 
    test/classes/differentiate_wrt_var.cpp 
    test/classes/homogenization_test.cpp 
    test/classes/start_system_test.cpp 
    test/classes/node_serialization_test.cpp 
    test/classes/patch_test.cpp 
    test/classes/slice_test.cpp 
    test/classes/m_hom_start_system.cpp 
    test/classes/class_test.cpp 
)


add_executable(b2_class_test ${B2_CLASS_TEST_SOURCES})
target_include_directories(b2_class_test PUBLIC
                                         ${CMAKE_CURRENT_SOURCE_DIR}/test/classes)
target_link_libraries(b2_class_test ${Boost_LIBRARIES} bertini2)
add_test(NAME b2_class_test COMMAND ${CMAKE_BINARY_DIR}/bin/b2_class_test)


set(B2_BLACKBOX_TEST
    test/blackbox/blackbox.cpp
    test/blackbox/zerodim.cpp
    test/blackbox/parsing.cpp
    test/blackbox/user_homotopy.cpp
)
add_executable(blackbox_test ${B2_BLACKBOX_TEST}) #Find these names in the Makemodules for the folder
target_link_libraries(blackbox_test ${Boost_LIBRARIES} bertini2)
add_test(NAME blackbox_test COMMAND ${CMAKE_BINARY_DIR}/bin/blackbox_test)

set(B2_CLASSES_TEST
    test/classes/boost_multiprecision_test.cpp
    test/classes/fundamentals_test.cpp
    test/classes/eigen_test.cpp
    test/classes/complex_test.cpp
    test/classes/function_tree_test.cpp
    test/classes/function_tree_transform.cpp
    test/classes/system_test.cpp
    test/classes/slp_test.cpp
    test/classes/differentiate_test.cpp
    test/classes/differentiate_wrt_var.cpp
    test/classes/homogenization_test.cpp
    test/classes/start_system_test.cpp
    test/classes/node_serialization_test.cpp
    test/classes/patch_test.cpp
    test/classes/slice_test.cpp
    test/classes/m_hom_start_system.cpp
    test/classes/class_test.cpp
)

set(B2_CLASSIC_TEST
    test/classes/boost_multiprecision_test.cpp
    test/classes/fundamentals_test.cpp
    test/classes/eigen_test.cpp
    test/classes/complex_test.cpp
    test/classes/function_tree_test.cpp
    test/classes/function_tree_transform.cpp
    test/classes/system_test.cpp
    test/classes/slp_test.cpp
    test/classes/differentiate_test.cpp
    test/classes/differentiate_wrt_var.cpp
    test/classes/homogenization_test.cpp
    test/classes/start_system_test.cpp
    test/classes/node_serialization_test.cpp
    test/classes/patch_test.cpp
    test/classes/slice_test.cpp
    test/classes/m_hom_start_system.cpp
    test/classes/class_test.cpp
)

set(B2_ENDGAMES_TEST
    test/endgames/endgames_test.cpp
    test/endgames/generic_interpolation.hpp
    test/endgames/interpolation.cpp
    test/endgames/generic_pseg_test.hpp
    test/endgames/amp_powerseries_test.cpp
    test/endgames/fixed_double_powerseries_test.cpp
    test/endgames/fixed_multiple_powerseries_test.cpp
    test/endgames/generic_cauchy_test.hpp
    test/endgames/amp_cauchy_test.cpp
    test/endgames/fixed_double_cauchy_test.cpp
    test/endgames/fixed_multiple_cauchy_test.cpp 
)

set(B2_GENERATING_TEST
    test/generating/mpfr_float.cpp
    test/generating/mpfr_complex.cpp
    test/generating/double.cpp
    test/generating/std_complex.cpp
    test/generating/generating_test.cpp 
)

set(B2_NAG_ALGORITHMS_TEST
    test/nag_algorithms/nag_algorithms_test.cpp
    test/nag_algorithms/zero_dim.cpp
    test/nag_algorithms/numerical_irreducible_decomposition.cpp
    test/nag_algorithms/trace.cpp 
)

set(B2_NAG_DATATYPES
	test/nag_datatypes/witness_set.cpp 
	test/nag_datatypes/nag_datatypes_test.cpp 
	test/nag_datatypes/numerical_irreducible_decomposition.cpp 
)

set(B2_POOLS_TEST
    test/pools/pool_test.cpp 
)

set(B2_SETTINGS_TEST
    test/settings/settings_test.cpp
)

set(B2_TRACKING_BASICS_TEST
    test/tracking_basics/newton_correct_test.cpp
    test/tracking_basics/euler_test.cpp
    test/tracking_basics/heun_test.cpp
    test/tracking_basics/higher_predictor_test.cp
    test/tracking_basics/tracking_basics_test.cpp
    test/tracking_basics/fixed_precision_tracker_test.cpp
    test/tracking_basics/amp_criteria_test.cpp
    test/tracking_basics/amp_tracker_test.cpp
    test/tracking_basics/path_observers.cpp
)


enable_testing()

# This is a template for all of the set variables in the beggining of this file, just headers, not sources

# To test, the /usr/local/include/bertini2 strcuture should mirror the structure of this folder (core/include)
