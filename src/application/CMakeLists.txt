include_directories(
	${PROJECT_SOURCE_DIR}
	${SDL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
)

add_library(application application.cpp camera_roam.cpp imageio.cpp gameobject.cpp component.cpp meshrenderer.cpp log.cpp)

if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI\"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	#add_definitions(-include \"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"); 
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include \"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")   
endif()
