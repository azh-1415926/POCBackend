cmake_minimum_required(VERSION 2.8.0)

set(CMAKE_CXX_STANDARD 17)

include(D:/local/vs-toolchain.cmake)

find_package(Drogon CONFIG REQUIRED)

include_directories(include)
aux_source_directory(src srcs)

add_executable(main main.cpp ${srcs})
target_link_libraries(main PRIVATE Drogon::Drogon)


file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR})