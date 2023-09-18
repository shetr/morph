

file(GLOB_RECURSE TESTS_SRC
    "src/**.cpp"
)

set(TESTS_INCLUDE 
    "src" 
    "../../engine/src"
    "../../extern/glm"
    "../../extern/googletest/googletest/include"
)

add_executable(unit_tests ${TESTS_SRC})

target_include_directories(unit_tests PUBLIC ${TESTS_INCLUDE})

target_link_libraries(unit_tests PRIVATE glm)
target_link_libraries(unit_tests PRIVATE gtest gmock gtest_main)
target_link_libraries(unit_tests PRIVATE morph_engine)

add_test(unit_tests unit_tests)
