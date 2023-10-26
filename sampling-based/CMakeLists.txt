cmake_minimum_required(VERSION 3.16)
project(sampling_proj LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(sampling_proj src/main.cpp src/utils.cpp)
target_include_directories(sampling_proj PRIVATE include)
target_link_libraries(sampling_proj PRIVATE sfml-graphics)
target_compile_features(sampling_proj PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET sampling_proj
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:sampling_proj>
        VERBATIM)
endif()

install(TARGETS sampling_proj)


# TESTING
enable_testing()

add_executable(
  hello_test
  src/hello_test.cc
  src/utils.cpp
)
target_include_directories(hello_test PRIVATE include)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)