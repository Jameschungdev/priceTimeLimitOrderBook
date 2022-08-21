cmake_minimum_required(VERSION 3.5)

project(priceTimeOrderLimitBook)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${SRCS}
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        )

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB SRCS_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

add_executable(
        hello_test
        ${SRCS_TESTS}
)
target_link_libraries(
        hello_test
        GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(hello_test)
        