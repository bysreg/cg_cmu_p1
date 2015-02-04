include_directories(
	${PROJECT_SOURCE_DIR}
)
add_library(scene mesh.cpp heightmap.cpp)
if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI\"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	#add_definitions(-include \"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"); 
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include \"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")   
endif()
