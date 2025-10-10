include(FetchGTest)
include(GoogleTest)

add_executable(unittests)
target_sources(unittests PRIVATE
  ${PROJECT_SOURCE_DIR}/src/stewkk/ptp/logic/input_test.cpp
  ${PROJECT_SOURCE_DIR}/src/stewkk/ptp/logic/monoid_test.cpp
  ${PROJECT_SOURCE_DIR}/src/stewkk/ptp/logic/dot_test.cpp
  ${PROJECT_SOURCE_DIR}/src/stewkk/ptp/logic/strong_connectivity_test.cpp
)
target_compile_features(unittests PRIVATE cxx_std_23)
set_target_properties(unittests PROPERTIES
  CXX_STANDART 23
  CXX_STANDART_REQUIRED YES
  CXX_EXTENSIONS YES
)
target_compile_options(unittests PRIVATE ${BASE_COMPILE_FLAGS})
target_link_options(unittests PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(unittests PRIVATE
  gmock_main
  stewkk::libptp
)
gtest_discover_tests(unittests)
