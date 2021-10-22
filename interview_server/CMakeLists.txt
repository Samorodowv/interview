cmake_minimum_required(VERSION 3.20)
project(interview_server)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system filesystem serialization  REQUIRED)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*")
include_directories(${Boost_INCLUDE_DIR})
add_executable(interview_server ${SOURCES} )


target_link_libraries(${PROJECT_NAME}
        PRIVATE ${OpenCV_LIBS}
        Threads::Threads
        ${Boost_LIBRARIES}
        )

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)