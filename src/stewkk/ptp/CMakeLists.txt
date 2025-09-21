add_executable(ptp main.cpp)
target_compile_features(ptp PRIVATE cxx_std_23)
set_target_properties(ptp PROPERTIES
  CXX_STANDART 23
  CXX_STANDART_REQUIRED YES
  CXX_EXTENSIONS YES
)
target_compile_options(ptp PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(ptp PRIVATE)
