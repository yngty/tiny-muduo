project(demo)

add_executable(demo main.cpp)

target_compile_definitions(demo PRIVATE MUDEBUG)
target_link_libraries(demo PRIVATE DUMUO::library)