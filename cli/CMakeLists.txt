# Add Boost dependency
find_package(Boost COMPONENTS program_options REQUIRED)

# Add fmt dependency
find_package(fmt CONFIG REQUIRED)

# Add Magic Enum dependency
find_package(magic_enum CONFIG REQUIRED)

# Add Nameof dependency
find_package(nameof CONFIG REQUIRED)

# Add spdlog dependency
find_package(spdlog CONFIG REQUIRED)

set(SOURCES
    ${CLI_SRC_DIR}/main.cpp
)

set(HEADERS
)

# Create executable file from sources
add_executable(${NAME_CLI} ${SOURCES} ${HEADERS})

target_include_directories(${NAME_CLI} PRIVATE
                           ${LIB_INCLUDE_DIR}
                           ${CLI_INCLUDE_DIR}
)

target_link_libraries(${NAME_CLI} PRIVATE
                      ${NAME_LIB}
                      Boost::boost
                      Boost::program_options
                      fmt::fmt
                      magic_enum::magic_enum
                      nameof::nameof
                      spdlog::spdlog
                      spdlog::spdlog_header_only
)