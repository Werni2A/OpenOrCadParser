# Add Catch2 dependency
find_package(Catch2 REQUIRED)

# Add fmt dependency
find_package(fmt CONFIG REQUIRED)

# Add Magic Enum dependency
find_package(magic_enum CONFIG REQUIRED)

# Add Nameof dependency
find_package(nameof CONFIG REQUIRED)

# Add spdlog dependency
find_package(spdlog CONFIG REQUIRED)

set(SOURCES
   # ${TEST_SRC_DIR}/test.cpp
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
                      fmt::fmt
                      Catch2::Catch2WithMain
                      magic_enum::magic_enum
                      nameof::nameof
                      spdlog::spdlog
                      spdlog::spdlog_header_only
                      ${NAME_LIB}
)

include(CTest)
include(Catch)
catch_discover_tests(${NAME_TEST}
                     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/generated_tests)

include(CatchShardTests)

catch_add_sharded_tests(${NAME_TEST}
  SHARD_COUNT 8
)