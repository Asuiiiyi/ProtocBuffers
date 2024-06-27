cmake_minimum_required(2.8.12.2)
project(PTB)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${PROJECT_SOURCE_DIR}/test
)

#根结点
add_subdirectory(test)


link_libraries(test)


# 添加可执行程序
add_executable(MyExecutable demo.cpp)