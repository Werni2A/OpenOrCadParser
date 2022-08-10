# Add Catch2 dependency
find_package(Catch2 REQUIRED)

# Add Magic Enum dependency
find_package(magic_enum CONFIG REQUIRED)

# Add Nameof dependency
find_package(nameof CONFIG REQUIRED)

# Add automatically generated test files
file(GLOB TEST_MISC_SRC ${TEST_SRC_DIR}/generated_tests/Test_*.cpp)

set(SOURCES
   ${TEST_SRC_DIR}/test.cpp
   ${TEST_MISC_SRC}
)

set(HEADERS
)

# Create executable file from sources
add_executable(${NAME_TEST} ${SOURCES} ${HEADERS})

target_include_directories(${NAME_TEST} PRIVATE
                           ${LIB_INCLUDE_DIR}
                           ${TEST_INCLUDE_DIR}
)

target_link_libraries(${NAME_TEST} PRIVATE
                      Catch2::Catch2WithMain
                      magic_enum::magic_enum
                      nameof::nameof
                      ${NAME_LIB}
)