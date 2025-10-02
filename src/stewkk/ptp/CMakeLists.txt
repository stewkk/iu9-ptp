add_library(libptp
  logic/input.cpp
  logic/monoid.cpp
  logic/dot.cpp
)
add_library(stewkk::libptp ALIAS libptp)
target_compile_features(libptp PRIVATE cxx_std_23)
set_target_properties(libptp PROPERTIES
  CXX_STANDART 23
  CXX_STANDART_REQUIRED YES
  CXX_EXTENSIONS YES
)
set_target_properties(libptp PROPERTIES OUTPUT_NAME stewkk_ptp)
target_include_directories(
  libptp PUBLIC $<INSTALL_INTERFACE:include>
                    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_options(libptp PRIVATE ${BASE_COMPILE_FLAGS})
target_link_options(libptp PRIVATE ${BASE_LINK_FLAGS})
target_link_libraries(libptp
)

add_executable(ptp main.cpp)
target_compile_features(ptp PRIVATE cxx_std_23)
set_target_properties(ptp PROPERTIES
  CXX_STANDART 23
  CXX_STANDART_REQUIRED YES
  CXX_EXTENSIONS YES
)
target_compile_options(ptp PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(ptp PRIVATE
  libptp
)
