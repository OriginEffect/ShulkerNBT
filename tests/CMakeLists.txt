add_executable(test_main test.cpp)

target_link_libraries(test_main PRIVATE ${SHULKER_NBT_LIB_NAME})
target_compile_features(test_main PRIVATE cxx_std_17)

add_test(NAME test_main COMMAND test_main)
