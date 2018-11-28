# TODO(darius98): This should be auto-generated from a list of extensions.

add_subdirectory(kktest_extensions/core_matchers)
add_subdirectory(kktest_extensions/feedback)

add_library(kktest $<TARGET_OBJECTS:kktest_impl>
    generated/kktest_main.cpp

    $<TARGET_OBJECTS:kktest_extension_core_matchers>
    $<TARGET_OBJECTS:kktest_extension_feedback>
)

target_link_libraries(kktest kktest_interface
    kktest_extension_core_matchers
    kktest_extension_feedback
)