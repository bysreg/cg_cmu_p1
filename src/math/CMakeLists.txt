include_directories(${PROJECT_SOURCE_DIR})
add_library(math camera.cpp color.cpp matrix.cpp quaternion.cpp vector.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI\"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")