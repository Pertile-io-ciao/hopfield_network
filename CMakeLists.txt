cmake_minimum_required(VERSION 3.28)
project(hopfield_network VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# warning
string(APPEND CMAKE_CXX_FLAGS " -fno-omit-frame-pointer -Wall -Wextra")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

# sanitizers in debug
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# internal library
add_library(hopfield_lib
    src/image_processor.cpp
    src/functions.cpp
    src/hebb.cpp
    src/recall.cpp
    src/graphics.cpp
)

target_link_libraries(hopfield_lib
    PRIVATE sfml-graphics sfml-window sfml-system
)

target_include_directories(hopfield_lib PUBLIC src)


# main executable
add_executable(hopfield_network src/main.cpp)
target_link_libraries(hopfield_network PRIVATE hopfield_lib)


# tests
if (BUILD_TESTING)
    add_executable(hopfield_network_tests
        tests/function_test.cpp
        tests/hebb_test.cpp
    )
    target_link_libraries(hopfield_network_tests PRIVATE hopfield_lib)

    add_test(NAME hopfield_network_tests COMMAND hopfield_network_tests)
endif()
