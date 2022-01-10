# Add Catch2 dependency
find_package(Catch2 REQUIRED)

# Add Magic Enum dependency
find_package(magic_enum CONFIG REQUIRED)

# Add Nameof dependency
find_package(nameof CONFIG REQUIRED)

set(SOURCES
   ${TEST_SRC_DIR}/catch_main.cpp
   ${TEST_SRC_DIR}/test.cpp
   
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
                      magic_enum::magic_enum
                      nameof::nameof
                      ${NAME_LIB}
)