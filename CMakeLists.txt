cmake_minimum_required(VERSION 2.8.0)

set(CMAKE_CXX_STANDARD 17)

include(D:/local/vs-toolchain.cmake)

find_package(Drogon CONFIG REQUIRED)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Found Python3: ${Python3_EXECUTABLE} (found version "${Python3_VERSION}")")

include_directories(include ${Python3_INCLUDE_DIRS})
aux_source_directory(src srcs)

add_executable(main main.cpp ${srcs})
target_link_libraries(main PRIVATE Drogon::Drogon ${Python3_LIBRARIES})


file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR}/Release)
file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR})

file(COPY tools/xc.exe DESTINATION ${CMAKE_BINARY_DIR}/Debug/tools)
file(COPY tools/xc.exe DESTINATION ${CMAKE_BINARY_DIR}/Release/tools)
file(COPY tools/xc.exe DESTINATION ${CMAKE_BINARY_DIR}/tools)