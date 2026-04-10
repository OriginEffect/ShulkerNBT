add_executable(test_main test.cpp)

target_link_libraries(test_main PRIVATE ${SHULKER_NBT_LIB_NAME})
target_compile_features(test_main PRIVATE cxx_std_17)

add_custom_command(TARGET test_main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/nbt
        $<TARGET_FILE_DIR:test_main>/nbt
)

add_custom_command(TARGET test_main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:test_main>
        $<TARGET_FILE_DIR:test_main>
        COMMAND_EXPAND_LISTS
)

add_test(NAME test_main COMMAND test_main)
