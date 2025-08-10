cmake_minimum_required(VERSION 3.28)
project(hopfield_network VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++20, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS " -fno-omit-frame-pointer -Wall -Wextra")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

# abilita l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")

# trova SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)


add_executable(hopfield_network
     src/main.cpp
     src/image_processor.cpp
     src/functions.cpp
     src/graphics.cpp
      )


# Linka SFML
target_link_libraries(hopfield_network PRIVATE sfml-graphics sfml-window sfml-system)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

  # aggiungi l'eseguibile sample.t
  # add_executable(image_processor.o image_processor.hpp image_processor.cpp)
  add_executable(hopfield_network.t src/functions.test.cpp src/functions.cpp)

  # Linka SFML
  target_link_libraries(hopfield_network.t PRIVATE sfml-graphics sfml-window sfml-system)

  # aggiungi l'eseguibile sample.t alla lista dei test
  add_test(NAME hopfield_network.t COMMAND hopfield_network.t )

  endif()


