# CMake generated Testfile for 
# Source directory: /home/marghe/hopfield_network
# Build directory: /home/marghe/hopfield_network/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[hopfield_network_tests]=] "/home/marghe/hopfield_network/build/Debug/hopfield_network_tests")
  set_tests_properties([=[hopfield_network_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/marghe/hopfield_network/CMakeLists.txt;58;add_test;/home/marghe/hopfield_network/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[hopfield_network_tests]=] "/home/marghe/hopfield_network/build/Release/hopfield_network_tests")
  set_tests_properties([=[hopfield_network_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/marghe/hopfield_network/CMakeLists.txt;58;add_test;/home/marghe/hopfield_network/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[hopfield_network_tests]=] "/home/marghe/hopfield_network/build/RelWithDebInfo/hopfield_network_tests")
  set_tests_properties([=[hopfield_network_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/marghe/hopfield_network/CMakeLists.txt;58;add_test;/home/marghe/hopfield_network/CMakeLists.txt;0;")
else()
  add_test([=[hopfield_network_tests]=] NOT_AVAILABLE)
endif()
