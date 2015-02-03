include_directories(
	${PROJECT_SOURCE_DIR}
)
add_library(scene mesh.cpp heightmap.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI\"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")